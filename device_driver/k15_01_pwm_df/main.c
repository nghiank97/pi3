#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/pwm.h>
#include <linux/err.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>

#define PWM_NAME 		"pwm_test"
#define PWM_PERIOD_NS 	40000 	// 	25k
#define PWM_DUTY_NS 	20000 	// 	1ms

static struct pwm_device *pwm;

static int pwm_probe(struct platform_device *pdev){
    int ret;
    const char *label;
    struct device *dev = &pdev->dev;
    
    pwm = pwm_get(dev, "pwm_led,dev");
    if (IS_ERR(pwm)) {
        return -1;
    }
    ret = pwm_config(pwm, PWM_PERIOD_NS, PWM_PERIOD_NS);
    if (ret < 0) {
        goto r_pwm;
    }
    pwm_enable(pwm);

	ret = device_property_read_string(dev, "status", &label);
	if(ret) {
		return -1;
	}
    return 0;
r_pwm:
    pwm_free(pwm);
    return ret;
}

static int pwm_remove(struct platform_device *pdev){
    pwm_config(pwm, 0, PWM_PERIOD_NS);
    pwm_disable(pwm);
    pwm_free(pwm);
    return 0;
}

static struct of_device_id pwm_ids[] = {
	{
		.compatible = "pwm_led,dev",
	}, { /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, pwm_ids);
static struct platform_driver pwm_driver = {
    .probe = pwm_probe,
    .remove = pwm_remove,
    .driver = {
        .name = PWM_NAME,
        .of_match_table = of_match_ptr(pwm_ids),
    },
};

module_platform_driver(pwm_driver);

MODULE_LICENSE("GPL");
