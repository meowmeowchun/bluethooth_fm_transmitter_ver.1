#include <Arduino.h>
#include "BluetoothA2DPSink.h"  // ESP32 A2DP藍牙接收
#include "driver/i2s.h"         // 用於I2S音訊傳輸

BluetoothA2DPSink a2dp_sink;

// I2S配置
const i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),  // 主模式，傳輸
    .sample_rate = 44100,                                // 標準音頻取樣率
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,        // 16-bit位元深度
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,        // 立體聲
    .communication_format = I2S_COMM_FORMAT_I2S_MSB,     // I2S通信格式
    .intr_alloc_flags = 0,                               // 預設中斷分配
    .dma_buf_count = 8,                                  // DMA buffer數量
    .dma_buf_len = 64,                                   // 每個buffer的長度
    .use_apll = false,                                   // 不使用APLL
    .tx_desc_auto_clear = true                           // 自動清除描述符
};

// I2S引腳配置
const i2s_pin_config_t pin_config = {
    .bck_io_num = 26,   // BCK引腳
    .ws_io_num = 25,    // LRCK引腳
    .data_out_num = 22, // DATA引腳
    .data_in_num = I2S_PIN_NO_CHANGE // 無數據輸入
};

// 回調函數，用於處理藍牙音訊數據
void audio_data_callback(const uint8_t *data, uint32_t len) {
    size_t bytes_written;
    i2s_write(I2S_NUM_0, data, len, &bytes_written, portMAX_DELAY);  // 將數據寫入I2S
}

void setup() {
    Serial.begin(9600);
    Serial.println("Starting Bluetooth A2DP Sink...");

    // 初始化I2S
    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &pin_config);

    // 啟動藍牙A2DP接收
    a2dp_sink.set_stream_reader(audio_data_callback);
    a2dp_sink.start("ESP32_BT_Audio");  // 藍牙裝置名稱

    Serial.println("Bluetooth A2DP Sink started.");
}


void loop() {
    // 音訊接收和傳輸在背景進行
}
