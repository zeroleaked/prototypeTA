#define FIXED_EXPOSURE 700

#define DIFFRACTION_ROW 512
#define COL_OFFSET 286
#define ROW_LENGTH 430

#define WL1_A 0.6908695652
#define WL1_B 202.0352174
#define INDEX_WL2_START 385
#define WL2_A 0.75
#define WL2_B 179.27
#define INDEX_WL3_START 472
#define WL3_A 0.686618705
#define WL3_B 209.1859712

#define SAMPLE_LENGTH 20 // max 257

typedef enum {
    NO_COM, // 0
    FIXED_EXPOSURE_COM, // 1
    VAR_EXPOSURE_COM, // 2
    BLANK_COM, // 3
    ABSORBANCE_COM, // 4
    ROW_DETECT_COM, // 5, for finding diffraction row
    VAR_GAIN_COM // for LED calibration
} command_t;

typedef struct {
    float * buf;          
    size_t len;
    uint16_t col_index;
} blank_t;