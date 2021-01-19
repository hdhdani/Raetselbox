/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "inttypes.h"
#include "stdio.h"
#include "display.h"
#include "neopixel.h"
//#include "gesturesens.h"
#include "tempsens.h"
#include "Adafruit_APDS9960.h"
#include "buttonmatrix.h"
#include "spielemanager.h"
#include "simonsays.h"
#include "gameconfig.h"
#include "tempspiel.h"
#include "farbspiel.h"
#include "asteroids.h"
#include "usart.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
volatile uint8_t buttonstate;

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  /* Objekte Periphärie*/
  Neopixel myneopixel;
  myneopixel.init( WS_Pin_GPIO_Port,WS_Pin_Pin, 8);
  Display oleddisplay;
  oleddisplay.init(hi2c1,0x78/*0x3D*/);
  Tempsens mytempsens;
  mytempsens.init(hi2c1, 0x40);
  Adafruit_APDS9960 gestsens;
   gestsens.begin(10, APDS9960_AGAIN_4X, 0x39,hi2c1,0x39);
   gestsens.enableColor(true);

 /*Konfigurationen für spiel*/
  Gameconfig mygameconfig; 
  HAL_Delay(10);
  

  mygameconfig.set( "ssrunden", 10,1);
  mygameconfig.set( "ssfarben", 4,1); 
  mygameconfig.set( "aswaves" ,3 ,1);
  



  /* Spiel Objekte */
  Spielemanger myspielemanger ;
  Simonsays mysimonsays(&myneopixel, &oleddisplay, & mygameconfig  );  //Pointer übergeben 
  Tempspiel tempspiel(&mytempsens, &oleddisplay,& mygameconfig  );
  Farbspiel farbspiel(&gestsens,&myneopixel,&oleddisplay,&mygameconfig);
  Asteroids asteroiden(oleddisplay, mygameconfig);

  oleddisplay.setfont(2);
  const char mychar[]= "Press  any   Key";
  oleddisplay.writestring(0,28,mychar);
  oleddisplay.updatescreen();
  buttonstate=getButton();
  uint8_t uartrec[2];
  char uartchange[] = "changed config";
  volatile int test [2];
  while (!buttonstate) // warte auf taster zum starten 
  {
    buttonstate=getButton();
    //auf uart read warten für konfig 

  HAL_UART_Receive(& huart2 , uartrec, 2, 100);

  if(uartrec[0] == 1)
  { 
    mygameconfig.set( "ssrunden", uartrec[1],1);
    test[0]= uartrec[0];
    test[1]= uartrec[1];
  }
  if(uartrec[0] == 2) 
  { 
    mygameconfig.set( "ssfarben",  uartrec[1],1); 
    test[0]= uartrec[0];
    test[1]= uartrec[1]; 
  }
  if(uartrec[0] == 3)
  {
    mygameconfig.set("aswaves",uartrec[1],1);
    test[0]= uartrec[0];
    test[1]= uartrec[1];
  }
  if(uartrec[0] != 0)
  {
    
    oleddisplay.cleanbuffer();
    oleddisplay.writestring(1,1,uartchange);
    oleddisplay.updatescreen();
  }


  uartrec[0] = 0;
  uartrec[1]=0; 
  }
  
  srand(HAL_GetTick()); // random seed für Zufallsfunktion einstellen -> "Zufall" durch Zeit bis Taster gedrückt wird
  oleddisplay.cleanbuffer();
  oleddisplay.updatescreen();

  myspielemanger.add_game(&mysimonsays); // simon says als Spiel hinzufügen 
  myspielemanger.add_game(&farbspiel);
  myspielemanger.add_game(&tempspiel); // Temperaturspiel hinzufügen
  myspielemanger.add_game(&asteroiden);
 
  while(1)
  {   // spiele/ Rätsel starten 
    myspielemanger.start(); 
  } //end while 



} //end main()



/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLL_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
