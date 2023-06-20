#define IOCTL_START_NUM 0x80
#define IOCTL_NUM1 IOCTL_START_NUM+1
#define IOCTL_NUM2 IOCTL_START_NUM+2
#define IOCTL_NUM3 IOCTL_START_NUM+3
#define IOCTL_NUM4 IOCTL_START_NUM+4

#define ALERT_IOCTL_NUM 'z'
#define PLAY _IOWR(ALERT_IOCTL_NUM, IOCTL_NUM1, unsigned long*)
#define STOP _IOWR(ALERT_IOCTL_NUM, IOCTL_NUM2, unsigned long*)
#define ON _IOWR(ALERT_IOCTL_NUM, IOCTL_NUM3, unsigned long*)
#define OFF _IOWR(ALERT_IOCTL_NUM, IOCTL_NUM4, unsigned long*)
