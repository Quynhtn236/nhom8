#include "main.h"

I2C_HandleTypeDef hi2c1;
#define SLAVE_ADDRESS_LCD 0x4E

void lcd_send_cmd(char cmd);
void lcd_init (void);
void lcd_send_string (char *str);
void lcd_send_data (char data);
void lcd_clear (void);
void lcd_put_cur (int row, int col);

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);


void lcd_send_cmd (char cmd)
{
		char data_u, data_l;
		uint8_t data_t[4];
		data_u = (cmd&0xf0);
		data_l = ((cmd<<4)&0xf0);
		data_t[0] = data_u|0x0C; //en=1, rs=0
		data_t[1] = data_u|0x08; //en=0, rs=0
		data_t[2] = data_l|0x0C; //en=1, rs=0
		data_t[3] = data_l|0x08; //en=0, rs=0
		HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}

void lcd_send_data (char data)
{
		char data_u, data_l;
		uint8_t data_t[4];
		data_u = (data&0xF0);
		data_l = ((data<<4)&0xF0);
		data_t[0] = data_u|0x0D; //en=1, rs=1
		data_t[1] = data_u|0x09; //en=0, rs=1
		data_t[2] = data_l|0x0D; //en=1, rs=1
		data_t[3] = data_l|0x09; //en=0, rs=1
		HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}

void lcd_init (void)
{
		lcd_send_cmd (0x33); 
		lcd_send_cmd (0x32); //thiet lap giao dien 4 bit
		HAL_Delay(50);
	
		lcd_send_cmd (0x28); // bat dau thiet lap chuc nang LCD
		HAL_Delay(50);
	
		lcd_send_cmd (0x01); //xoa hien thi tren LCD
		HAL_Delay(50);
	
		lcd_send_cmd (0x06); //thiet lap che do nhap, cho phep con tro sang phai sau khi moi ky tu duoc viet
		HAL_Delay(50);
	
		lcd_send_cmd (0x0c); //b?t hien thi tren ma LCD va an con tro van ban
		HAL_Delay(50);
	
		lcd_send_cmd (0x02); //dua con tro ve vi tri dau tien tren man LCD va thiet lap du lieu là 0
		HAL_Delay(50);
	
		lcd_send_cmd (0x80); //dat con tro ve vi tri dau tien tren man hinh LCD de bat dau viet du lieu
}

void lcd_send_string (char *str)
{
	    while (*str) {
        lcd_send_data(*str);  
        str++;  
    }
}

void lcd_clear (void)
{
		lcd_send_cmd (0x80);
		for (int i=0; i<70; i++)
		{
			lcd_send_data (' ');
		}
}

void lcd_put_cur (int row, int col)
{
	    if (row == 1)
    {
        lcd_send_cmd(0x80 + col);  
    }
    else if (row == 2)
    {
        lcd_send_cmd(0xC0 + col);  
    }
}

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_I2C1_Init();
	lcd_init();
int i;
  while (1)
  {
			lcd_put_cur(1,3);
			lcd_send_string("ARM 19 CT4");
			HAL_Delay(3000);
			lcd_clear();
		for(i=0;i<=8;i++){
					lcd_put_cur(1,3);
					lcd_send_string("ARM 19 CT4");
					HAL_Delay(250);
					lcd_clear();
					HAL_Delay(150);	
		}
		for(i=0;i<=16;i++){
			lcd_put_cur(1,3);
			lcd_send_string("HELLO WORLD");
			lcd_put_cur(2,i);
			lcd_send_string("ARM 19 CT4");
			HAL_Delay(500);
			lcd_clear();
		}
  }
}


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}


static void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }


}


static void MX_GPIO_Init(void)
{

  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();


}


void Error_Handler(void)
{

  __disable_irq();
  while (1)
  {
  }

}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
