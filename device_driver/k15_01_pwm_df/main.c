#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/pwm.h>
#include <linux/err.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>

#define PWM_NAME 		"pwm_test"
#define LED_NAME 		"led_test"
#define PWM_PERIOD_NS 	400000 	// 	25k
#define PWM_DUTY_NS 	200000 	// 	1ms

static struct pwm_device *pwm;

static int pwm_probe(struct platform_device *pdev){
    int ret;
    pwm = pwm_request(0, PWM_NAME);
    if (IS_ERR(pwm)) {
        dev_err(&pdev->dev, "failed to request PWM\n");
        return PTR_ERR(pwm);
    }
    ret = pwm_config(pwm, PWM_DUTY_NS, PWM_PERIOD_NS);
    if (ret < 0) {
        dev_err(&pdev->dev, "failed to configure PWM\n");
        goto err_release_pwm;
    }
    pwm_enable(pwm);
    return 0;
err_release_pwm:
    pwm_free(pwm);
    return ret;
}

static int pwm_remove(struct platform_device *pdev){
    pwm_config(pwm, 0, PWM_PERIOD_NS);
    pwm_disable(pwm);
    pwm_free(pwm);
    return 0;
}

static struct platform_driver pwm_driver = {
    .driver = {
        .name = LED_NAME,
    },
    .probe = pwm_probe,
    .remove = pwm_remove,
};

static int __init etx_driver_init(void){
    int ret;
    ret = platform_driver_register(&pwm_driver);
    if (ret < 0) {
        pr_err("\t failed to register PWM platform driver\n");
        return ret;
    }
	pr_info("\t inserted\n");
    return 0;
}

static void __exit etx_driver_exit(void){
    platform_driver_unregister(&pwm_driver);
	pr_info("\t exit\n");
}

module_init(etx_driver_init);
module_exit(etx_driver_exit);

MODULE_LICENSE("GPL");
