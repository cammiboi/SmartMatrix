#ifndef _LAYER_FOREGROUND_H_
#define _LAYER_FOREGROUND_H_

#include "Layer.h"
#include "MatrixCommon.h"

// scroll text
const int textLayerMaxStringLength = 100;

typedef enum ScrollMode {
    wrapForward,
    bounceForward,
    bounceReverse,
    stopped,
    off,
    wrapForwardFromLeft,
} ScrollMode;


// font
#include "MatrixFontCommon.h"

template <typename RGB>
class SMLayerForeground : public SM_Layer<RGB> {
    public:
        SMLayerForeground(uint8_t * bitmap, uint8_t width, uint8_t height);
        void frameRefreshCallback();
        void getRefreshPixel(uint8_t x, uint8_t y, RGB &refreshPixel);

        void setScrollColor(const RGB & newColor);
        colorCorrectionModes ccmode = cc48;
        void setColorCorrection(colorCorrectionModes mode);

        //bitmap size is 32 rows (supporting maximum dimension of screen height in all rotations), by 32 bits
        // double buffered to prevent flicker while drawing
        uint8_t * foregroundBitmap;

        void stopScrollText(void);
        void clearForeground(void);
        void displayForegroundDrawing(bool waitUntilComplete);
        void handleForegroundDrawingCopy(void);
        void drawForegroundPixel(int16_t x, int16_t y, bool opaque);
        void drawForegroundChar(int16_t x, int16_t y, char character, bool opaque = true);
        void drawForegroundString(int16_t x, int16_t y, const char text [], bool opaque = true);
        void drawForegroundMonoBitmap(int16_t x, int16_t y, uint8_t width, uint8_t height, uint8_t *bitmap, bool opaque = true);
        void setForegroundFont(fontChoices newFont);
        int getScrollStatus(void) const;
        void setScrollMinMax(void);
        void scrollText(const char inputtext[], int numScrolls);
        void updateScrollText(const char inputtext[]);

        void setScrollMode(ScrollMode mode);
        void setScrollSpeed(unsigned char pixels_per_second);
        void setScrollFont(fontChoices newFont);
#define setScrollOffsetFromEdge setScrollOffsetFromTop // backwards compatibility
        void setScrollOffsetFromTop(int offset);
        void setScrollStartOffsetFromLeft(int offset);

    private:
        void redrawForeground(void);
        // todo: move somewhere else
        static bool getBitmapPixelAtXY(uint8_t x, uint8_t y, uint8_t width, uint8_t height, const uint8_t *bitmap);
        void updateForeground(void);
        bool getForegroundPixel(uint8_t hardwareX, uint8_t hardwareY, RGB &xyPixel);

        RGB textcolor = {0xff, 0xff, 0xff};

        unsigned char currentframe = 0;
        char text[textLayerMaxStringLength];

        unsigned char textlen;
        int scrollcounter = 0;

        int fontTopOffset = 1;
        int fontLeftOffset = 1;
        bool majorScrollFontChange = false;

        // scrolling
        ScrollMode scrollmode = bounceForward;
        unsigned char framesperscroll = 4;

        volatile bool foregroundCopyPending = false;

        const bitmap_font *scrollFont = &apple5x7;

        // these variables describe the text bitmap: size, location on the screen, and bounds of where it moves
        unsigned int textWidth;
        int scrollMin, scrollMax;
        int scrollPosition;

        bitmap_font *foregroundfont = (bitmap_font *) &apple3x5;
};

#include "Layer_Foreground_Impl.h"

#endif

