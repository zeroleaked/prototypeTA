#define FIXED_EXPOSURE 400
#define FIXED_GAIN 0

#define DIFFRACTION_ROW 452

#define COL_OFFSET 395
#define ROW_LENGTH 418

#define WL1_A 0.76578313253012
#define WL1_B 97.3809638554218
#define INDEX_WL2_START 484
#define WL2_A 0.805555555555556
#define WL2_B 78.1311111111112
#define INDEX_WL3_START 565
#define WL3_A 0.676879432624114
#define WL3_B 150.833120567376


#define FILTER_LENGTH 4

#define SAMPLE_LENGTH 1 // max 257
#define LAMP_PIN 15



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