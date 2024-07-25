#include "../include/IR.h"
#include <stdbool.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>


#define IR_OUT DT_ALIAS(dtout)
#define IR_OUT_2 DT_ALIAS(dtout2)


#if !DT_NODE_HAS_STATUS(IR_OUT, okay)
#error "Unsupported board: IR 1 devicetree alias is not defined"
#endif

#if !DT_NODE_HAS_STATUS(IR_OUT_2, okay)
#error "Unsupported board: IR 2 devicetree alias is not defined"
#endif


static const struct gpio_dt_spec left_pin = GPIO_DT_SPEC_GET(IR_OUT, gpios);
static const struct gpio_dt_spec front_pin = GPIO_DT_SPEC_GET(IR_OUT_2, gpios);


static ir ir_left = {.d_out = left_pin};
static ir ir_front = {.d_out = front_pin};
static ir_data_t ir_data;


#define ADC_RESOLUTION         12
#define ADC_GAIN               ADC_GAIN_1
#define ADC_REFERENCE          ADC_REF_INTERNAL
#define ADC_CHANNEL_ID         1
#define ADC_CHANNEL_ID_FRONT   3
#define BUFFER_SIZE            1

static const struct device *adc_dev;
static const struct device *adc_dev_front;
static struct adc_channel_cfg channel_cfg;
static int16_t sample_buffer[BUFFER_SIZE];
static int16_t front_buffer[BUFFER_SIZE];

static void configure_adc(void) {
    adc_dev = device_get_binding("ADC_1");
    if (!adc_dev) {
        printk("ADC device not found\n");
        return;
    }

    adc_dev_front = device_get_binding("ADC_2");
    if (!adc_dev_front) {
        printk("ADC front device not found\n");
        return;
    }

    channel_cfg.channel_id = ADC_CHANNEL_ID;
    channel_cfg.differential = 0;
    channel_cfg.gain = ADC_GAIN;
    channel_cfg.reference = ADC_REFERENCE;
    channel_cfg.acquisition_time = ADC_ACQ_TIME_DEFAULT;

    if (adc_channel_setup(adc_dev, &channel_cfg) != 0) {
        printk("Setting up of the ADC channel failed\n");
        return;
    }

    channel_cfg.channel_id = ADC_CHANNEL_ID_FRONT;
    if (adc_channel_setup(adc_dev_front, &channel_cfg) != 0) {
        printk("Setting up of the ADC front channel failed\n");
        return;
    }
}

static int read_adc(void) {
    struct adc_sequence sequence = {
        .channels    = BIT(ADC_CHANNEL_ID),
        .buffer      = sample_buffer,
        .buffer_size = sizeof(sample_buffer),
        .resolution  = ADC_RESOLUTION,
    };

    int ret = adc_read(adc_dev, &sequence);
    if (ret < 0) {
        printk("ADC read failed with error %d\n", ret);
        return ret;
    }
    
    return sample_buffer[0];
}

static int read_adc_front(void) {
    struct adc_sequence sequence = {
        .channels    = BIT(ADC_CHANNEL_ID_FRONT),
        .buffer      = front_buffer,
        .buffer_size = sizeof(front_buffer),
        .resolution  = ADC_RESOLUTION,
    };

    int ret = adc_read(adc_dev_front, &sequence);
    if (ret < 0) {
        printk("ADC front read failed with error %d\n", ret);
        return ret;
    }
    printk("Front IR: %d \n", front_buffer[0]);
    return front_buffer[0];
}

void init_IR(void) {
    int ret;

    ret = gpio_pin_configure_dt(&ir_left.d_out, GPIO_INPUT);
    if (ret < 0) {
        printk("Error %d: failed to configure IR_left pin\n", ret);
        return;
    }

    ret = gpio_pin_configure_dt(&ir_front.d_out, GPIO_INPUT);
    if (ret < 0) {
        printk("Error %d: failed to configure IR_front pin\n", ret);
        return;
    }

    configure_adc();
    printk("IR sensors initialized\n");
}


int16_t adc_value;
int16_t adc_front_value;
ir_data_t read_IR(void) {

    bool wall_left = gpio_pin_get_dt(&ir_left.d_out);
    bool wall_front = gpio_pin_get_dt(&ir_front.d_out);

    
    adc_value = read_adc();    
    adc_front_value = read_adc_front();
    ir_data.left_ir_data = wall_left;
    ir_data.front_ir_data = wall_front;
    ir_data.right_analog_data = adc_value;
    ir_data.front_analog_data = adc_front_value;
    // printk("ADC reading: %d\n", adc_value);

    return ir_data;
}

int16_t return_analog(){
    return ir_data.right_analog_data;
}

int16_t return_analog_front(){
    return ir_data.front_analog_data;
}