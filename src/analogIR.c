
#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/adc.h>

#define ADC_RESOLUTION         12
#define ADC_GAIN               ADC_GAIN_1
#define ADC_REFERENCE          ADC_REF_INTERNAL
#define ADC_CHANNEL_ID         1
#define BUFFER_SIZE            1

static const struct device *adc_dev;
static struct adc_channel_cfg channel_cfg;
static int16_t sample_buffer[BUFFER_SIZE];

static void configure_adc(void) {
    adc_dev = device_get_binding("ADC_1");
    if (!adc_dev) {
        printk("ADC device not found\n");
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

void main(void) {
    int16_t adc_value;

    configure_adc();

    while (1) {
        adc_value = read_adc();
        printk("ADC reading: %d\n", adc_value);
        k_sleep(K_MSEC(100));
    }
}
