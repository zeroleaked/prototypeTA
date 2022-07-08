#define FIXED_EXPOSURE 200

#define DIFFRACTION_ROW 500

#define COL_OFFSET 339
#define ROW_LENGTH 418

#define WL1_A 0.747764705882354
#define WL1_B 146.481176470588
#define INDEX_WL2_START 430
#define WL2_A 0.725
#define WL2_B 156.27
#define INDEX_WL3_START 520
#define WL3_A 0.706962962962964
#define WL3_B 165.649259259259


#define FILTER_LENGTH 4

#define SAMPLE_LENGTH 10 // max 257

typedef enum {
    NO_COM, // 0
    FIXED_EXPOSURE_COM, // 1
    VAR_EXPOSURE_COM, // 2
    BLANK_COM, // 3
    ABSORBANCE_COM, // 4
    ROW_DETECT_COM, // 5, for finding diffraction row
    VAR_GAIN_COM, // for LED calibration
    BLANK_FILTER_COM,
    ABSORBANCE_FILTER_COM
} command_t;

typedef struct {
    float * buf;          
    size_t len;
    uint16_t col_index;
} blank_t;