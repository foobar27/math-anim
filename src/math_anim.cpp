
#include <iostream>
#include <stdexcept>

#include <SkData.h>
#include <SkImage.h>
#include <SkStream.h>
#include <SkSurface.h>
#include <SkCanvas.h>

void raster(int width, int height, void (*draw)(SkCanvas&), const char* path)
{
	sk_sp<SkSurface> rasterSurface = SkSurface::MakeRasterN32Premul(width, height);
	if (!rasterSurface)
		throw std::runtime_error("can't create surface");

	SkCanvas* rasterCanvas = rasterSurface->getCanvas();
	if (!rasterCanvas)
		throw std::runtime_error("can't get canvas");

	draw(*rasterCanvas);

	sk_sp<SkImage> img(rasterSurface->makeImageSnapshot());
	if (!img)
		throw std::runtime_error("can't make image snapshot");

	sk_sp<SkData> png(img->encode());
	if (!png)
		throw std::runtime_error("can't encode png");

	SkFILEWStream out(path);
	out.write(png->data(), png->size());
}

void draw(SkCanvas* canvas) {
    const SkScalar scale = 256.0f;
    const SkScalar R = 0.45f * scale;
    const SkScalar TAU = 6.2831853f;
    SkPath path;
    path.moveTo(R, 0.0f);
    for (int i = 1; i < 7; ++i) {
        SkScalar theta = 3 * i * TAU / 7;
        path.lineTo(R * cos(theta), R * sin(theta));
    }
    path.close();
    SkPaint p;
    p.setAntiAlias(true);
    canvas->clear(SK_ColorWHITE);
    canvas->translate(0.5f * scale, 0.5f * scale);
    canvas->drawPath(path, p);
}

void draw2(SkCanvas* canvas) {
    SkPaint paint;
    paint.setAntiAlias(true);
    SkPath path;
    path.moveTo(124, 108);
    path.lineTo(172, 24);
    path.addCircle(50, 50, 30);
    path.moveTo(36, 148);
    path.quadTo(66, 188, 120, 136);
    canvas->drawPath(path, paint);
    paint.setStyle(SkPaint::kStroke_Style);
    paint.setColor(SK_ColorBLUE);
    paint.setStrokeWidth(3);
    canvas->drawPath(path, paint);
}

int main() {
    const char *testFileName = "test.png";
    const int WIDTH = 100;
    const int HEIGHT = 100;
    
    std::cout << "Writing test output to " << testFileName << std::endl;
    raster(WIDTH, HEIGHT, drawTest, testFileName);
}
