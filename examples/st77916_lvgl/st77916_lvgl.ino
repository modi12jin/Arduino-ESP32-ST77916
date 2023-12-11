#include <lvgl.h>
#include "demos/lv_demos.h"
#include "ST77916_LCD.h"
#include "CST816T.h"

#define QSPI_SCK 4
#define QSPI_0 3
#define QSPI_1 10
#define QSPI_2 9
#define QSPI_3 6
#define QSPI_CS 5
#define LCD_RST 0

#define I2C_SDA 8
#define I2C_SCL 11
#define TP_INT 14
#define TP_RST 13

#define LED 1  // 背光

ST77916_LCD lcd = ST77916_LCD(QSPI_SCK, QSPI_0, QSPI_1, QSPI_2, QSPI_3, QSPI_CS, LCD_RST);
CST816T touch(I2C_SDA, I2C_SCL, TP_RST, TP_INT);

//更改为您的屏幕分辨率
static const uint32_t screenWidth = 360;
static const uint32_t screenHeight = 360;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[2][screenWidth * 20];

//显示刷新
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {

  lcd.lcd_PushColors(area->x1, area->y1, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1, &color_p->full);

  lv_disp_flush_ready(disp);  //告诉lvgl刷新完成
}

//读取触摸
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {

  bool touched;
  uint8_t gesture;
  uint16_t touchX, touchY;

  touched = touch.getTouch(&touchX, &touchY, &gesture);

  if (!touched) {
    data->state = LV_INDEV_STATE_REL;
  } else {
    data->state = LV_INDEV_STATE_PR;

    /*Set the coordinates*/
    data->point.x = touchX;
    data->point.y = touchY;
  }
}

void setup() {
  Serial.begin(115200);
  lcd.begin();
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);  // 高电平
  touch.begin();
  lv_init();

  lv_disp_draw_buf_init(&draw_buf, buf[0], buf[1], screenWidth * 20);

  //初始化显示
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  //将以下行更改为您的显示分辨率
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  //初始化输入设备驱动程序
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);

  lv_demo_widgets();  //小部件
  // lv_demo_keypad_encoder(); //键盘编码器
  // lv_demo_benchmark(); //基准测试
  // lv_demo_stress(); //压力测试
  // lv_demo_music();　//音乐
}

void loop() {
  lv_timer_handler();
  delay(5);
 }