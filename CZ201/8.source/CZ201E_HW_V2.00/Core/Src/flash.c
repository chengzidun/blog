/*
 * flash.c
 *
 *  Created on: 2019��3��11��
 *      Author: cheng
 */

#include "flash.h"
#include <string.h>

uint8_t flash_init(void)
{
	return 0;
}

/**
  * @brief  write page, no erase
  * @param  address: address of the FLASH Memory, must be a multiple of 1024
  * @param  size: max 1024 byte
  * @retval is succeed
  */
uint8_t flash_write_page(uint32_t address, uint8_t* pdata, uint32_t size)
{
	uint32_t err;
	uint64_t pages[FLASH_PAGE_SIZE/sizeof(uint64_t)];
	uint32_t i;

	if(address < FLASH_USER_START_ADDR || address > FLASH_USER_END_ADDR || size == 0 || size > FLASH_PAGE_SIZE) return 1;
	if((address % FLASH_PAGE_SIZE) != 0)  return 1;

	memset((uint8_t*)pages, 0xFF, sizeof(pages));
	memcpy((uint8_t*)pages, pdata, size);

	HAL_FLASH_Unlock();
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_PGERR | FLASH_SR_WRPERR);

	for(i = 0; i < FLASH_PAGE_SIZE/sizeof(uint64_t); i++)
	{
		err = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, address, pages[i]);
		if(err != HAL_OK) break;
		address += sizeof(uint64_t);
	}

	HAL_FLASH_Lock();

	return err;
}

/**
  * @brief  write data, no check, no erase
  * @param  address: address of the FLASH Memory, must be a multiple of 1024
  * @param  size: write to byte
  * @retval is succeed
  */
uint8_t flash_write_no_check(uint32_t address, uint8_t* pdata, uint32_t size)
{
	uint16_t pageremain;

	if(address < FLASH_USER_START_ADDR || address > FLASH_USER_END_ADDR || size == 0) return 1;
	if((address % FLASH_PAGE_SIZE) != 0)  return 1;

	pageremain = FLASH_PAGE_SIZE - address % FLASH_PAGE_SIZE; //����ʣ����ֽ���
	if(size <= pageremain) pageremain = size;				//������1024���ֽ�

	while(1)
	{
		if(flash_write_page(address, pdata, pageremain)) return 1;
		if(size == pageremain) break;			//д�������
		else //size > pageremain
		{
			pdata += pageremain;
			address += pageremain;

			size -= pageremain;			  		//��ȥ�Ѿ�д���˵��ֽ���
			if(size > FLASH_PAGE_SIZE)
				pageremain = FLASH_PAGE_SIZE; 	//һ�ο���д��1024���ֽ�
			else pageremain = size; 	  		//����1024���ֽ���
		}
	}

	return 0;
}

/**
  * @brief  write data, check, erase
  * @param  address: address of the FLASH Memory, must be a multiple of 1024
  * @param  size: write to byte
  * @param  pbuffer: temporary buffer
  * @retval is succeed
  */
uint8_t flash_write_data(uint32_t address, uint8_t* pdata, uint32_t size, uint8_t* ptempbuf)
{
	uint32_t page_pos;
	uint32_t page_off;
	uint32_t page_remain;
	uint32_t i;
	uint8_t *pbuffer = ptempbuf;

	if(address < FLASH_USER_START_ADDR || address > FLASH_USER_END_ADDR || size == 0) return 1;

   	if(pbuffer == NULL) return 1;

 	page_pos = address / FLASH_PAGE_SIZE;	//ҳ��ַ
	page_off = address % FLASH_PAGE_SIZE;	//��ҳ�ڵ�ƫ��
	page_remain = FLASH_PAGE_SIZE - page_off;	//ҳʣ��ռ��С
 	if(size <= page_remain) page_remain = size;	//������2048���ֽ�

	while(1)
	{
		if(flash_read_data(page_pos*FLASH_PAGE_SIZE, pbuffer, FLASH_PAGE_SIZE))	//��������ҳ������
		{
			return 1;
		}
		for(i = 0; i < page_remain; i++)//У������
		{
			if(pbuffer[page_off+i] != 0xFF) break;	//��Ҫ����
		}
		if(i < page_remain)//��Ҫ����
		{
			if(flash_erase_page(page_pos*FLASH_PAGE_SIZE, 1))	//�������ҳ
			{
				return 1;
			}

			for(i = 0; i < page_remain; i++)	//����
				pbuffer[i+page_off] = pdata[i];

			if(flash_write_no_check(page_pos*FLASH_PAGE_SIZE, pbuffer, FLASH_PAGE_SIZE))	//д����������
			{
				return 1;
			}
		}
		else
		{
			if(flash_write_no_check(address, pdata, page_remain))	//д�Ѿ������˵�,ֱ��д��ҳʣ������.
			{
				return 1;
			}
		}

		if(size == page_remain) break;//д�������
		else//д��δ����
		{
			page_pos++;		//ҳ��ַ��1
			page_off = 0;	//ƫ��λ��Ϊ0

		   	pdata += page_remain;  				//ָ��ƫ��
			address += page_remain;				//д��ַƫ��
		   	size -= page_remain;				//�ֽ����ݼ�
			if(size > FLASH_PAGE_SIZE)
				page_remain = FLASH_PAGE_SIZE;	//��һ��ҳ����д����
			else page_remain = size;			//��һ��ҳ����д����
		}
	}

	return 0;
}

uint8_t flash_read_data(uint32_t address, uint8_t* pdata, uint32_t size)
{
	uint32_t i;
	uint8_t *paddr = (uint8_t*)address;

	for(i = 0; i < size; i++)
	{
		*pdata = *paddr;
		pdata++;
		paddr++;
	}

	return 0;
}

/**
  * @brief  erase page
  * @param  address: address of the FLASH Memory
  * @param  page_nb: number of page
  * @retval is succeed
  */
uint8_t flash_erase_page(uint32_t address, uint32_t page_nb)
{
	FLASH_EraseInitTypeDef erase_init;
	uint32_t err = 0, hal_err;
	uint32_t page;

	page = get_page(address);

	if(address < FLASH_USER_START_ADDR || address > FLASH_USER_END_ADDR || page_nb == 0) return 1;
	if((page + page_nb-1) > get_page(FLASH_USER_END_ADDR)) return 1;

	HAL_FLASH_Unlock();
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_PGERR | FLASH_SR_WRPERR);

	erase_init.TypeErase = FLASH_TYPEERASE_PAGES;
	erase_init.PageAddress = address;
	erase_init.NbPages = page_nb;
	hal_err = HAL_FLASHEx_Erase(&erase_init, &err);

	HAL_FLASH_Lock();

	return hal_err;
}

/**
  * @brief  Gets the page of a given address
  * @param  address: Address of the FLASH Memory
  * @retval The page of a given address
  */
uint32_t get_page(uint32_t address)
{
  uint32_t page = 0;

  page = (address - FLASH_BASE) / FLASH_PAGE_SIZE;

  return page;
}

/**
  * @brief This function handles Flash global interrupt.
  */
void FLASH_IRQHandler(void)
{
  /* USER CODE BEGIN FLASH_IRQn 0 */

  /* USER CODE END FLASH_IRQn 0 */
  HAL_FLASH_IRQHandler();
  /* USER CODE BEGIN FLASH_IRQn 1 */

  /* USER CODE END FLASH_IRQn 1 */
}

