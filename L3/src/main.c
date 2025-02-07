/******************************************************************************/
/*!                 Header Files                                              */

#include <zephyr/kernel.h>

// include the printk function file header
#include <zephyr/sys/printk.h>

// include the common header file that will be used in conjunction with BMI323 drivers
#include <common.h>

// include bmi323 API function haeders
#include <bmi323.h>

// include the math library
#include <math.h>

// this code was taken from the '<BMI323_SensorAPI/examples/accel/accel.c>' example

/******************************************************************************/
/*!         Macros definition                                                 */

/*! Earth's gravity in m/s^2 */
#define GRAVITY_EARTH  (9.80665f)

/******************************************************************************/
/*!           Static Function Declaration                                     */

/*!
 *  @brief This internal API is used to set configurations for accel.
 *
 *  @param[in] dev       : Structure instance of bmi3_dev.
 *
 *  @return Status of execution.
 */
static int8_t set_accel_config(struct bmi3_dev *dev);

/*!
 *  @brief This internal function converts lsb to meter per second squared for 16 bit accelerometer for
 *  range 2G, 4G, 8G or 16G.
 *
 *  @param[in] val       : LSB from each axis.
 *  @param[in] g_range   : Gravity range.
 *  @param[in] bit_width : Resolution for accel.
 *
 *  @return Accel values in meter per second squared.
 */
static float lsb_to_g(int16_t val, int8_t g_range, uint8_t bit_width);

/*!
 *  @brief This internal API is used to set configurations for gyro.
 *
 *  @param[in] dev       : Structure instance of bmi3_dev.
 *
 *  @return Status of execution.
 */
static int8_t set_gyro_config(struct bmi3_dev *dev);

/*!
 *  @brief This function converts lsb to degree per second for 16 bit gyro at
 *  range 125, 250, 500, 1000 or 2000dps.
 *
 *  @param[in] val       : LSB from each axis.
 *  @param[in] dps       : Degree per second.
 *  @param[in] bit_width : Resolution for gyro.
 *
 *  @return Degree per second.
 */
static float lsb_to_dps(int16_t val, float dps, uint8_t bit_width);


/******************************************************************************/
/*!            Functions                                                      */


/**
 * Main Funciton
 */
int main(void)
{
    /* Sensor initialization configuration. */
    struct bmi3_dev dev = { 0 };

    /* Status of API are returned to this variable. */
    int8_t rslt;

    /* Create an instance of sensor data structure. */
    struct bmi3_sensor_data acc_sensor_data = { 0 };
    struct bmi3_sensor_data gyr_sensor_data = { 0 };

    /* Initialize the interrupt status of accel. */
    uint16_t sens_status = 0;

    // dummy variable for printing current line
    uint8_t indx = 0;

    /** acceleration values in m/s^2 */
    float acc_x = 0, acc_y = 0, acc_z = 0;

    /** gyroscope values in degree/s */
    float gyr_x = 0, gyr_y = 0, gyr_z = 0;

    /* Select both accel and gyro sensor. */
    acc_sensor_data.type = BMI323_ACCEL;
    gyr_sensor_data.type = BMI323_GYRO;

    /* Function to select interface between SPI and I2C, according to that the device structure gets updated.
     * Interface reference is given as a parameter
     * For I2C : BMI3_I2C_INTF
     * For SPI : BMI3_SPI_INTF
     */
    rslt = bmi3_interface_init(&dev, BMI3_I2C_INTF);
    bmi3_error_codes_print_result("bmi3_interface_init", rslt);

    /* Initialize bmi323. */
    rslt = bmi323_init(&dev);
    bmi3_error_codes_print_result("bmi323_init", rslt);

    if (rslt == BMI323_OK)
    {
        /* Accel configuration settings. */
        rslt = set_accel_config(&dev);
        bmi3_error_codes_print_result("accel config set", rslt);

        /* Gyroscope configuration settings. */
        rslt = set_gyro_config(&dev);
        

        if (rslt == BMI323_OK)
        {
            printk("\n\r");
            printk("----------------------------------------------------------------------------------\n\r");
            printk("Data set, Acc_Raw_X, Acc_Raw_Y, Acc_Raw_Z, Acc_g_X, Acc_g_Y, Acc_g_Z\n\r");
            printk("Data set, Gyr_Raw_X, Gyr_Raw_Y, Gyr_Raw_Z, Gyr_dps_X, Gyr_dps_Y, Gyr_dps_Z\n\r");
            printk("----------------------------------------------------------------------------------\n\r");


            // infinite loop
            while (1)
            {
                /* To get the status of accel data ready interrupt. */
                rslt = bmi323_get_sensor_status(&sens_status, &dev);
                bmi3_error_codes_print_result("bmi323_get_int1_status", rslt);

                /* To check the accel data ready status and print the status.*/
                /* The problem with the below line is that it waits till both Acc and Gyro data are ready */
                if ((sens_status & BMI3_STATUS_DRDY_ACC) && (sens_status && BMI3_STATUS_DRDY_GYR))
                {
                    /* Get accelerometer data for x, y and z axis. 1 is the number of readings to get*/
                    rslt = bmi323_get_sensor_data(&acc_sensor_data, 1, &dev);
                    bmi3_error_codes_print_result("Get sensor data", rslt);

                    /* Get gyro data for x, y and z axis */
                    rslt = bmi323_get_sensor_data(&gyr_sensor_data, 1, &dev);
                    bmi3_error_codes_print_result("Get sensor data", rslt);

                    /* Converting lsb to g for 16 bit accelerometer at 4G range. */
                    acc_x = lsb_to_g(acc_sensor_data.sens_data.acc.x, 4, dev.resolution);
                    acc_y = lsb_to_g(acc_sensor_data.sens_data.acc.y, 4, dev.resolution);
                    acc_z = lsb_to_g(acc_sensor_data.sens_data.acc.z, 4, dev.resolution);
                    
                    /* Converting lsb to degree per second for 16 bit gyro at 500dps range. */
                    gyr_x = lsb_to_dps(gyr_sensor_data.sens_data.gyr.x, (float)500, dev.resolution);
                    gyr_y = lsb_to_dps(gyr_sensor_data.sens_data.gyr.y, (float)500, dev.resolution);
                    gyr_z = lsb_to_dps(gyr_sensor_data.sens_data.gyr.z, (float)500, dev.resolution);

                    /* Print the data in g units for serial monitor. */
                    /* %4.2d means to use at least 4 places for the decimal part and exactly 2 places for the fracitonal part*/
                    printk("------------------------------------\n\r");
                    printk("%d, %d, %d, %d, %4.2f g, %4.2f g, %4.2f g\n\r",
                           indx,
                           acc_sensor_data.sens_data.acc.x,
                           acc_sensor_data.sens_data.acc.y,
                           acc_sensor_data.sens_data.acc.z,
                           acc_x,
                           acc_y,
                           acc_z);
                    printk("%d, %d, %d, %d, %4.2f dsp, %4.2f dsp, %4.2f dsp\n\r",
                           indx,
                           gyr_sensor_data.sens_data.gyr.x,
                           gyr_sensor_data.sens_data.gyr.y,
                           gyr_sensor_data.sens_data.gyr.z,
                           gyr_x,
                           gyr_y,
                           gyr_z);
                    // printk("------------------------------------\n\r");
                    

                   /* Print the data in g units for serial plotter. */
                   /* Note: no special characters are allowed as labels like '_' or '-', etc... */
                    // printk("ACCx%4.2f", acc_x);
                    // printk("ACCy%4.2f", acc_y);
                    // printk("ACCz%4.2f", acc_z);
                    // printk("GYROx%4.2f", gyr_x);
                    // printk("GYROy%4.2f", gyr_y);
                    // printk("GYROz%4.2f", gyr_z); 

                    // printk("\n\r");
                
                    indx++;
                }

                // sample a new reading every 20 mS (running at ~50HZ)
                k_msleep(20);
            }
        }
    }

    return 0;
}

/*!
 * @brief This internal API is used to set configurations for accel.
 * 
 */
static int8_t set_accel_config(struct bmi3_dev *dev)
{
    /* Status of API are returned to this variable. */
    int8_t rslt;

    /* Structure to define accelerometer configuration. */
    struct bmi3_sens_config config;

    /* Configure the type of feature. */
    config.type = BMI323_ACCEL;

    /* Get default configurations for the type of feature selected. */
    rslt = bmi323_get_sensor_config(&config, 1, dev);
    bmi3_error_codes_print_result("bmi323_get_sensor_config", rslt);

    if (rslt == BMI323_OK)
    {

        /* NOTE: The user can change the following configuration parameters according to their requirement. */
        /* Output Data Rate. By default ODR is set as 100Hz for accel. */
        config.cfg.acc.odr = BMI3_ACC_ODR_100HZ;

        /* Gravity range of the sensor (+/- 2G, 4G, 8G, 16G). */
        config.cfg.acc.range = BMI3_ACC_RANGE_4G;

        /* The Accel bandwidth coefficient defines the 3 dB cutoff frequency in relation to the ODR. */
        /* In other words, it defines the bandwidth limits upon which the low pass filter will act */
        config.cfg.acc.bwp = BMI3_ACC_BW_ODR_QUARTER;

        /* Set number of average samples for accel. in other words, its output is the average of the most recent 8 samples*/
        config.cfg.acc.avg_num = BMI3_ACC_AVG8;

        /* Enable the accel mode where averaging of samples
        * will be done based on above set bandwidth and ODR.
        * Note : By default accel is disabled. The accel will get enable by selecting the mode.
        */
        config.cfg.acc.acc_mode = BMI3_ACC_MODE_NORMAL;

        /* Set the accel configurations. */
        rslt = bmi323_set_sensor_config(&config, 1, dev);
        bmi3_error_codes_print_result("bmi323_set_sensor_config", rslt);
        
    }

    return rslt;
}

/*!
 * @brief This function converts lsb to meter per second squared for 16 bit accelerometer at
 * range 2G, 4G, 8G or 16G.
 * 
 * @details: 
 *      this function works as follows -:
 *          'val' is the returned number from the accelerometer which is by default a 16-bit value 
 *                 ranging between -32768 and 32767 all-inclusive.
 *          we will first normalize this number to be in the range of '-1 to 1'. we will need to just divide 'val' by 
 *              its maximum value which is stored in the variable 'half_scale' according to the bitwidth of 'val'
 *          then depending on the sensor range whether it's '±2G', '±4G', '±8G' or '±16G'. We will map the values '-1 to 1' to 
 *              the range of 'g_range' by simple multiplication. if 
 */     
static float lsb_to_g(int16_t val, int8_t g_range, uint8_t bit_width)
{
    double power = 2;

    float half_scale = (float)((pow((double)power, (double)bit_width) / 2.0f));

    return (val * g_range) / half_scale;
}


/*!
 *  @brief This internal API is used to set configurations for gyro.
 */
static int8_t set_gyro_config(struct bmi3_dev *dev)
{

    /* Status of API are returned to this variable. */
    int8_t rslt;

    /* Structure to define the type of sensor and its configurations. */
    struct bmi3_sens_config config;

    /* Configure the type of feature. */
    config.type = BMI323_GYRO;

    /* Get default configurations for the type of feature selected. */
    rslt = bmi323_get_sensor_config(&config, 1, dev);
    bmi3_error_codes_print_result("Get sensor config", rslt);

    if (rslt == BMI323_OK)
    {

        /* The user can change the following configuration parameters according to their requirement. */
        /* Output Data Rate. By default ODR is set as 100Hz for gyro. */
        config.cfg.gyr.odr = BMI3_GYR_ODR_100HZ;

        /* Gyroscope Angular Rate Measurement Range. By default the range is 2000dps. */
        config.cfg.gyr.range = BMI3_GYR_RANGE_500DPS;

        /*  The Gyroscope bandwidth coefficient defines the 3 dB cutoff frequency in relation to the ODR
            *  Value   Name      Description
            *    0   odr_half   BW = gyr_odr/2
            *    1  odr_quarter BW = gyr_odr/4
            */
        config.cfg.gyr.bwp = BMI3_GYR_BW_ODR_HALF;

        /* By default the gyro is disabled. Gyro is enabled by selecting the mode. */
        config.cfg.gyr.gyr_mode = BMI3_GYR_MODE_NORMAL;

        /* Value    Name    Description
            *  000     avg_1   No averaging; pass sample without filtering
            *  001     avg_2   Averaging of 2 samples
            *  010     avg_4   Averaging of 4 samples
            *  011     avg_8   Averaging of 8 samples
            *  100     avg_16  Averaging of 16 samples
            *  101     avg_32  Averaging of 32 samples
            *  110     avg_64  Averaging of 64 samples
            */
        config.cfg.gyr.avg_num = BMI3_GYR_AVG1;

        /* Set the gyro configurations. */
        rslt = bmi323_set_sensor_config(&config, 1, dev);
        bmi3_error_codes_print_result("Set sensor config", rslt);
        
    }

    return rslt;
}

/*!
 * @brief This function converts lsb to degree per second for 16 bit gyro at
 * range 125, 250, 500, 1000 or 2000dps.
 * 
 * @details
 *      this function works as follows -:
 *          'val' is the returned number from the gyroscope which is by default a 16-bit value 
 *                 ranging between -32768 and 32767 all-inclusive.
 *          we will first normalize this number to be in the range of '-1 to 1'. we will need to just divide 'val' by 
 *              its maximum value which is stored in the variable 'half_scale' according to the bitwidth of 'val'
 *          then depending on the sensor range whether it's '±125dps', '±250dps', '±500dps', '±1000dps' or '±2000'. We will map the values '-1 to 1' to 
 *              the range of 'dps' by simple multiplication.
 */
static float lsb_to_dps(int16_t val, float dps, uint8_t bit_width)
{
    double power = 2;

    float half_scale = (float)((pow((double)power, (double)bit_width) / 2.0f));

    return (dps / (half_scale)) * (val);
}