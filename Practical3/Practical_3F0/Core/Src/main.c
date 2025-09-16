/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdint.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define MAX_ITER 100
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MAX_CYCLES 0xFFFFFFFFULL
#define WIDTH 128
#define HEIGHT 128
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
// TODO: Define variables you think you might need
//  - Performance timing variables (e.g execution time, throughput, pixels per second, clock cycles)
//checksum
volatile uint64_t checksum = 0;

//execution time
volatile uint32_t start_time = 0;
volatile uint32_t end_time = 0;
volatile uint32_t execution_time = 0;

//cycles
volatile uint32_t start_cycle = 0;
volatile uint32_t end_cycle = 0;
volatile uint64_t total_cycles = 0;
volatile uint32_t overflow_count = 0;
volatile uint32_t start_overflow = 0;
volatile uint32_t end_overflow = 0;

//throughput
volatile uint32_t total_pixels = 0;
volatile uint32_t throughput = 0;

// Fixed-point arithmetic constants
#define SHIFT 16
#define S (1LL << 16) // 65536 scaling
#define FIXED_2_5 ((int64_t)(2.5 * S))
#define FIXED_3_5 ((int64_t)(3.5 * S))
#define FIXED_2_0 ((int64_t)(2 * S))
#define FIXED_4_0 ((int64_t)(4 * S))
#define FIXED_1_0 ((int64_t)(1 * S))

// Benchmarking control variables
volatile int current_max_iter = 0;
volatile int current_image_size = 0;
volatile int test_index = 0;

// Image dimensions for testing (square images)
int imageDimensions[] = {128, 160, 192, 224, 256};

// Max iterations values for testing 
int maxIterValues[] = {100, 250, 500, 750, 1000};


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

// TODO: Define any function prototypes you might need such as the calculate Mandelbrot function among others
uint64_t calculate_mandelbrot_double(int width, int height, int max_iterations);
uint64_t calculate_mandelbrot_fixed_point_arithmetic(int width, int height, int max_iterations);

// Task functions
void run_task1(void);
void run_task2(void);
void run_task3(void);
void run_task6(void);

// Timer functions prototypes
void init_TIM2(void);
void TIM2_IRQHandler(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

  /* USER CODE BEGIN 1 */
  init_TIM2();
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
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    //run_task1();
    //run_task2(); 
    run_task3();
    //run_task6();

    // Delay before repeating the loop
    HAL_Delay(1000);

  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pins : PB0 PB1 PB2 PB3
                           PB4 PB5 PB6 PB7 */
  GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
// TODO: Function signatures you defined previously , implement them here

/**
 * Method that initialises timer 2 to measure clock cycles
 */
void init_TIM2(void)
{
  // Enable TIM2
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

  // Configure timer parameters to measure clock cycles
  TIM2->PSC = 0;          // tick at CPU clock speed
  TIM2->ARR = 0xFFFFFFFF; // Set ARR to max
  TIM2->CNT = 0;          // Reset counter

  // Enable overflow interrupt
  TIM2->DIER |= TIM_DIER_UIE;
  NVIC_EnableIRQ(TIM2_IRQn); // for interrupt handler


  overflow_count = 0;
  TIM2->CR1 |= TIM_CR1_CEN; // start the timer
}

/**
 * Interrupt handler for TIM2
 */
void TIM2_IRQHandler(void)
{
  // If overflow occurs
  if (TIM2->SR & TIM_SR_UIF)
  {
    overflow_count++;
    TIM2->SR &= ~TIM_SR_UIF; // Clear overflow flag
  }
}

/*
* Helper function for fixed-point multiplication
*/
static inline int64_t mult(int64_t a, int64_t b)
{
  return (a * b) >> SHIFT;
}

/*
* Mandelbrot using fixed-point arithmetic
*/
uint64_t calculate_mandelbrot_fixed_point_arithmetic(int width, int height, int max_iterations)
{
  uint64_t result = 0;

  // Precompute outside loop
  int64_t preX = (FIXED_3_5) / width;
  int64_t preY = (FIXED_2_0) / height;

  for (int y = 0; y < height; y++)
  {
    // y0 = (y / height) * 2.0 - 1.0
    int64_t y0 = (y * preY) - FIXED_1_0;
    for (int x = 0; x < width; x++)
    {
      // x0 = (x / width) * 3.5 - 2.5
      int64_t x0 = (x * preX) - FIXED_2_5;

      int64_t xi = 0;
      int64_t yi = 0;

      int64_t iter = 0;

      while ((iter < max_iterations) && (mult(xi, xi) + mult(yi, yi) <= FIXED_4_0))
      {
        int64_t temp = mult(xi, xi) - mult(yi, yi);

        // yi = 2 * xi * yi + y0
        yi = mult(FIXED_2_0, mult(xi, yi)) + y0;

        xi = temp + x0;
        iter++;
      }

      result = result + iter;
    }
  }
  return result;
}

uint64_t calculate_mandelbrot_double(int width, int height, int max_iterations)
{
  uint64_t mandelbrot_sum = 0;
  // TODO: Complete the function implementation
  for (int y = 0; y < height; y++)
  {
    double y0 = (double)y * 2.0 / height - 1.0; // Scale y coordinate

    for (int x = 0; x < width; x++)
    {
      double x0 = (double)x * 3.5 / width - 2.5; // Scale x coordinate
      double xi = 0.0;
      double yi = 0.0;
      int iteration = 0;

      while (iteration < max_iterations && (xi * xi + yi * yi <= 4.0))
      {
        double temp = xi * xi - yi * yi + x0; // Calculate x component
        yi = 2.0 * xi * yi + y0;              // Calculate y component
        xi = temp;                            // Update xi
        iteration++;
      }
      // Accumulate iteration count for this pixel
      mandelbrot_sum += iteration;
    }
  }
  return mandelbrot_sum;
}

/*
* Task 1: Test with fixed MAX_ITER=100 and image sizes from Practical 1B
*/
void run_task1(void)
{
    // Visual indicator: Turn on LED0 to signal processing start
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);

    // Task 1: Test with fixed MAX_ITER=100 and image sizes from Practical 1B
    for (int size_idx = 0; size_idx < 5; size_idx++) {
        current_image_size = imageDimensions[size_idx];
        current_max_iter = 100; // Fixed for Task 1
        
        // Record the start time
        start_time = HAL_GetTick();
        
        // Call the Mandelbrot Function
        checksum = calculate_mandelbrot_double(current_image_size, current_image_size, current_max_iter);
        
        // Record the end time
        end_time = HAL_GetTick();
        
        // Calculate the execution time
        execution_time = end_time - start_time;
        
        // Set breakpoint HERE to record results for each test
        // Use Live Expressions: current_max_iter, current_image_size, checksum, execution_time
        
        // Brief pause between tests
        HAL_Delay(5000);
    }

    // Visual indicator: Turn on LED1 to signal processing end
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);

    // Keep the LEDs ON for 2s
    HAL_Delay(2000);

    // Turn OFF LEDs
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
}


/**
 * Task 2: Testing the impact of maximum iteration variable 
 */
void run_task2(void)
{
    // Visual indicator: Turn on LED0 to signal processing start
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);

    // Task 2: Loop through all MAX_ITER values and image sizes
    for (int iter_idx = 0; iter_idx < 5; iter_idx++) {
        current_max_iter = maxIterValues[iter_idx];
        
        for (int size_idx = 0; size_idx < 5; size_idx++) {
            current_image_size = imageDimensions[size_idx];
            test_index = iter_idx * 5 + size_idx; // 0-24 for tracking progress
            
            // Record the start time
            start_time = HAL_GetTick();
            
            // Call the Mandelbrot Function - CHANGE THIS TO TEST DIFFERENT VERSIONS
            checksum = calculate_mandelbrot_double(current_image_size, current_image_size, current_max_iter);
            
            // Record the end time
            end_time = HAL_GetTick();
            
            // Calculate the execution time
            execution_time = end_time - start_time;
            
            // Set breakpoint HERE to record results for each test
            // Use Live Expressions: current_max_iter, current_image_size, checksum, execution_time
            
            // Brief pause between tests (optional)
            HAL_Delay(1000);
        }
    }

    // Visual indicator: Turn on LED1 to signal processing end
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);

    // Keep the LEDs ON for 2s
    HAL_Delay(2000);

    // Turn OFF LEDs
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
}

/**
 * Task 3
 */
void run_task3(void)
{
    // Visual indicator: Turn on LED0 to signal processing start
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);

    // Task 3: Test with fixed MAX_ITER=100 and measure extended metrics
    for (int size_idx = 0; size_idx < 5; size_idx++) {
        current_image_size = imageDimensions[size_idx];
        current_max_iter = 100; // Fixed for Task 3
        
        // Calculate total pixels for this test
        total_pixels = current_image_size * current_image_size;
        
        // Record the start time (wall-clock time)
        start_time = HAL_GetTick();
        
        // Measure start cycles and overflow count
        start_cycle = TIM2->CNT;
        start_overflow = overflow_count;
        
        // Call the Mandelbrot Function
        checksum = calculate_mandelbrot_double(current_image_size, current_image_size, current_max_iter);
        
        // Measure end cycles and overflow count
        end_cycle = TIM2->CNT;
        end_overflow = overflow_count;
        
        // Record the end time (wall-clock time)
        end_time = HAL_GetTick();
        
        // Calculate the execution time in milliseconds
        execution_time = end_time - start_time;
        
        // Calculate number of CPU clock cycles
        if (end_overflow > start_overflow)
        {
            // Overflows occurred
            
            // Calculate number of cycles from start until counter wraps
            total_cycles = MAX_CYCLES - start_cycle;
            
            // Calculate cycles for full periods between overflows
            total_cycles += (uint64_t)(end_overflow - start_overflow - 1) * MAX_CYCLES;
            
            // Add remaining partial cycles after last overflow
            total_cycles += end_cycle;
        }
        else
        {
            // No overflow
            total_cycles = end_cycle - start_cycle;
        }
        
        // Calculate throughput (pixels per second)

        //Calculate total number of pixels
        total_pixels = imageDimensions[size_idx] * imageDimensions[size_idx];

        if (execution_time > 0)
        {
            throughput = (total_pixels * 1000) / execution_time; // Convert ms to seconds
        }
        else
        {
            throughput = 0; // Prevent division by 0
        }
        
        // Set breakpoint HERE to record results for each test
        // Use Live Expressions: current_image_size, checksum, execution_time, total_cycles, throughput
        
        // Brief pause between tests
        HAL_Delay(5000);
    }

    // Visual indicator: Turn on LED1 to signal processing end
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);

    // Keep the LEDs ON for 2s
    HAL_Delay(2000);

    // Turn OFF LEDs
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
}

/**
 * Task 6: Compiler optimisations
 */
void run_task6(void)
{
    // Visual indicator: Turn on LED0 to signal processing start
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);

    // Test with current compiler optimization level
    // You need to rebuild with different -O flags to test different levels
    
    for (int size_idx = 0; size_idx < 5; size_idx++) 
    {
        current_image_size = imageDimensions[size_idx];
        current_max_iter = 100; // Fixed for Task 6
        
        // Record start time
        start_time = HAL_GetTick();
        
        // Calculate Mandelbrot set
        checksum = calculate_mandelbrot_double(current_image_size, current_image_size, current_max_iter);
        
        // Record end time
        end_time = HAL_GetTick();
        execution_time = end_time - start_time;
        
        // Set breakpoint HERE to record results for each test
        // Use Live Expressions: current_image_size, checksum, execution_time

        // Brief pause between tests
        HAL_Delay(1000);
    }

    // Visual indicator: Turn on LED1 to signal processing completion
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);

    // Keep LEDs on for 2 seconds
    HAL_Delay(2000);

    // Turn off LEDs
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
}

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
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