#include "esp_timer.h"
#include "esp_camera.h"
#include "img_converters.h"
#include "Arduino.h"

#include "tatypes.h"

typedef struct {
    uint32_t filesize;
    uint32_t reserved;
    uint32_t fileoffset_to_pixelarray;
    uint32_t dibheadersize;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitsperpixel;
    uint32_t compression;
    uint32_t imagesize;
    uint32_t ypixelpermeter;
    uint32_t xpixelpermeter;
    uint32_t numcolorspallette;
    uint32_t mostimpcolor;
} bmp_header_t;

uint8_t getPixel(camera_fb_t * fb, uint16_t row, uint16_t col) {
    return fb->buf[row * fb->width + col]; 
}

float getWavelength(uint16_t index) {
    if (index < INDEX_WL2_START) {
        return WL1_A*(index) + WL1_B;
    }
    else if (index < INDEX_WL3_START) {
        return WL2_A*(index) + WL2_B;
    }
    else {
        return WL3_A*(index) + WL3_B;
    }
}

esp_err_t command_handler(command_t command, uint16_t arg1, blank_t * blank) {
    switch(command) {
        case FIXED_EXPOSURE_COM: {
            Serial.printf("DEFAULT EXPOSURE %d\n", FIXED_EXPOSURE);
            sensor_t * s = esp_camera_sensor_get();
            s->set_aec_value(s, FIXED_EXPOSURE);

            camera_fb_t * fb;

            uint16_t accumulate[ROW_LENGTH] = {0};
            for (int j=0; j<SAMPLE_LENGTH; j++) {
                fb = esp_camera_fb_get(); // bottleneck
                for (int i=0; i<ROW_LENGTH; i++) {
                    uint8_t val = getPixel(fb, DIFFRACTION_ROW, COL_OFFSET+i);
                    accumulate[i] += val;
                }

                esp_camera_fb_return(fb);
            }
            
            Serial.println("i\twl\tval");
            for (int i=0; i<ROW_LENGTH; i++) {
                float average = accumulate[i]/ (float) SAMPLE_LENGTH;
                float wavelength = getWavelength(COL_OFFSET+i);
                Serial.printf("%d\t%f\t%f\n", COL_OFFSET+i, wavelength, average);
            }

            break; /* optional */
        }
        
        case VAR_EXPOSURE_COM: {
            Serial.printf("EXPOSURE %d\n", arg1);
            sensor_t * s = esp_camera_sensor_get();
            s->set_aec_value(s, arg1);

            camera_fb_t * fb;

            uint16_t accumulate[ROW_LENGTH] = {0};
            for (int j=0; j<SAMPLE_LENGTH; j++) {
                fb = esp_camera_fb_get(); // bottleneck
                for (int i=0; i<ROW_LENGTH; i++) {
                    uint8_t val = getPixel(fb, DIFFRACTION_ROW, COL_OFFSET+i);
                    accumulate[i] += val;
                }

                esp_camera_fb_return(fb);
            }
            
            Serial.println("i\twl\tval");
            for (int i=0; i<ROW_LENGTH; i++) {
                float average = accumulate[i]/(float) SAMPLE_LENGTH;
                float wavelength = getWavelength(COL_OFFSET+i);
                Serial.printf("%d\t%f\t%f\n", COL_OFFSET+i, wavelength, average);
            }

            break; /* optional */
        }
        
        case BLANK_COM: {
            Serial.printf("TAKING BASELINE\n");
            sensor_t * s = esp_camera_sensor_get();
            s->set_aec_value(s, FIXED_EXPOSURE);

            camera_fb_t * fb;

            uint16_t accumulate[ROW_LENGTH] = {0};
            for (int j=0; j<SAMPLE_LENGTH; j++) {
                fb = esp_camera_fb_get(); // bottleneck
                for (int i=0; i<ROW_LENGTH; i++) {
                    uint8_t val = getPixel(fb, DIFFRACTION_ROW, COL_OFFSET+i);
                    accumulate[i] += val;
                }

                esp_camera_fb_return(fb);
            }
            
            Serial.println("wl\tval");
            for (int i=0; i<ROW_LENGTH; i++) {
                float average = accumulate[i]/(float) SAMPLE_LENGTH;
                float wavelength = getWavelength(COL_OFFSET+i);
                Serial.printf("%.2f\t%.2f\n", wavelength, average);
                blank->buf[i] = average;
            }

            break; /* optional */
        }
        
        case ABSORBANCE_COM: {
            Serial.printf("TAKING ABSORBANCE\n");
            sensor_t * s = esp_camera_sensor_get();
            s->set_aec_value(s, FIXED_EXPOSURE);

            camera_fb_t * fb;

            uint16_t accumulate[ROW_LENGTH] = {0};
            for (int j=0; j<SAMPLE_LENGTH; j++) {
                fb = esp_camera_fb_get(); // bottleneck
                for (int i=0; i<ROW_LENGTH; i++) {
                    uint8_t val = getPixel(fb, DIFFRACTION_ROW, COL_OFFSET+i);
                    accumulate[i] += val;
                }

                esp_camera_fb_return(fb);
            }
            
            Serial.println("wl\tsample\tbaseline\tabsorbance");
            for (int i=0; i<ROW_LENGTH; i++) {
                float average = accumulate[i]/(float) SAMPLE_LENGTH;
                float wavelength = getWavelength(COL_OFFSET+i);
                float baseline = blank->buf[i];
                float absorbance;
                if (average >= baseline) {
                    absorbance = 0;
                }
                else {
                    absorbance = log10(baseline/average);
                }
                Serial.printf("%.2f\t%.2f\t%.2f\t%.2f\n", wavelength, average, baseline, absorbance);
            }

            break; /* optional */
        }
        
        case VAR_GAIN_COM: {
            Serial.printf("GAIN %d\n", arg1);
            sensor_t * s = esp_camera_sensor_get();
            s->set_agc_gain(s, arg1);

            camera_fb_t * fb;

            uint16_t accumulate[ROW_LENGTH] = {0};
            for (int j=0; j<SAMPLE_LENGTH; j++) {
                fb = esp_camera_fb_get(); // bottleneck
                for (int i=0; i<ROW_LENGTH; i++) {
                    uint8_t val = getPixel(fb, DIFFRACTION_ROW, COL_OFFSET+i);
                    accumulate[i] += val;
                }

                esp_camera_fb_return(fb);
            }
            
            Serial.println("i\twl\tval");
            for (int i=0; i<ROW_LENGTH; i++) {
                float average = accumulate[i]/(float) SAMPLE_LENGTH;
                float wavelength = getWavelength(COL_OFFSET+i);
                Serial.printf("%d\t%f\t%f\n", COL_OFFSET+i, wavelength, average);
            }

            s->set_agc_gain(s, 0);

            break; /* optional */
        }

        case ROW_DETECT_COM: {
            Serial.printf("TOTAL FOR EACH ROW\n");
            sensor_t * s = esp_camera_sensor_get();
            s->set_aec_value(s, FIXED_EXPOSURE);
            camera_fb_t * fb;
            fb = esp_camera_fb_get();

            Serial.println("i\ttotal");
            uint32_t max_val = 0;
            uint16_t max_i;
            for (int i=0; i<768; i++) {
                uint32_t val = 0;
                for (int j=0; j<ROW_LENGTH; j++) {
                    val += getPixel(fb, i, COL_OFFSET+j);
                }
                Serial.printf("%d\t%d\n", i, val);
                if (val > max_val) {
                    max_val = val;
                    max_i = i;
                }
            }

            esp_camera_fb_return(fb);
            Serial.printf("max row = %d\n", max_i);
            break; /* optional */
        }
    }
}