#include <Arduino.h>

TaskHandle_t taskHandle[3];
QueueHandle_t xQueue;

typedef struct
{
 int id;
 int data[10];
} QMsg_t;

void TaskQueueSend1(void *args) {

  QueueHandle_t queue = (QueueHandle_t)args;

  QMsg_t msg1;

  msg1.id = 1;
  msg1.data[0] = 0;
  msg1.data[1] = 1;

  while (true) {

    xQueueSend(queue, (void*)&msg1, 0);
    printf("TaskQueueSend 1\r\n");
    vTaskDelay(100);
    msg1.data[0] += 1;
    msg1.data[1] += 2;
  }
}

void TaskQueueSend2(void *args) {

  QueueHandle_t queue = (QueueHandle_t)args;

  QMsg_t msg2;

  msg2.id = 2;
  msg2.data[0] = 2;
  msg2.data[1] = 3;

  while (true) {

    xQueueSend(queue, (void*)&msg2, 0);
    printf("TaskQueueSend 2\r\n");
    vTaskDelay(100);
    msg2.data[0] += 4;
    msg2.data[1] += 8;
  }
}

void TaskQueueRecv(void *args) {

  QueueHandle_t queue = (QueueHandle_t)args;
  QMsg_t msg;
  UBaseType_t itemNums;
  BaseType_t ret;

  while (true) {
    itemNums = uxQueueMessagesWaiting(queue);
    printf("Items = %d\r\n", itemNums);
    if (itemNums) {
      ret = xQueueReceive(queue, &msg, 0);
      if (ret == pdTRUE) {
        printf("TaskRecv, id = %d data[0] = %d data[1] = %d \r\n", msg.id, msg.data[0], msg.data[1]);
      }
      else {
        printf("TaskRecv, not received.\r\n");
      }
    }
    vTaskDelay(50);
  }
}

void setup() {

  xQueue = xQueueCreate(10, sizeof(QMsg_t));
  xTaskCreatePinnedToCore(TaskQueueSend1, "TaskQueueSend1", 4096, xQueue, 10, &taskHandle[1], APP_CPU_NUM);
  xTaskCreatePinnedToCore(TaskQueueSend2, "TaskQueueSend2", 4096, xQueue, 10, &taskHandle[1], APP_CPU_NUM);
  xTaskCreatePinnedToCore(TaskQueueRecv, "TaskQueueRecv", 4096, xQueue, 11, &taskHandle[0], APP_CPU_NUM);
}

void loop() {
}