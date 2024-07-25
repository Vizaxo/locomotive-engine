#include "PCH.h"

#include "Renderer2D.h"

#include "renderer/ImGuiUtils.h"
#include "types/Pointers.h"

#include "d2d1.h"

struct D2DRelease {
	void operator()(void** resource) { SafeRelease(*(IUnknown**)resource); }
};

static OwningPtr<ID2D1Factory, true, D2DRelease> pD2DFactory = nullptr;
static OwningPtr<ID2D1HwndRenderTarget, true, D2DRelease> renderTarget = nullptr;
static OwningPtr<ID2D1SolidColorBrush, true, D2DRelease> blackBrush = nullptr;
static Scene2D scene;

OwningPtr<ID2D1HwndRenderTarget, true, D2DRelease> createRenderTarget(HWND hwnd) {
	RECT rc;
	GetClientRect(hwnd, &rc);

	OwningPtr<ID2D1HwndRenderTarget, true, D2DRelease> pRT = nullptr;
	HRASSERT(pD2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(), 
		D2D1::HwndRenderTargetProperties(hwnd, D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)), 
		&pRT.getRaw()));

	return std::move(pRT);
}

OwningPtr<ID2D1SolidColorBrush, false, D2DRelease> createBlackBrush() {
	ID2D1SolidColorBrush* ret;
	renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &ret);
	return ret;
}


void Renderer2D::init(HWND hwnd) {
	HRASSERT(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory.getRaw()));
	renderTarget = std::move(createRenderTarget(hwnd));
	blackBrush = std::move(createBlackBrush().getNullable());
}

void Renderer2D::renderScene(float delta) {
	static float accDelta = 0;
	float prevAccDelta = accDelta;
	accDelta += delta;

	static const float spawnInterval = 0.1;
	if (floor(accDelta / spawnInterval) != floor(prevAccDelta / spawnInterval))
		scene.circles.emplace_back(Circle({ {randf() * 200 + 400, randf() * 200 + 400}, 20, {1, 1, 0}, v2f({(float)(randf() * 5.0), (float)(randf() * 5.0)}), 1.0 }));

	Renderer2D::tickPhysics(&scene, delta);
	renderTarget->BeginDraw();
	renderTarget->Clear();

	float strokeWidth = 10.0;
	for (Circle& circle : scene.circles) {
		renderTarget->FillEllipse(D2D1::Ellipse({circle.pos.x, circle.pos.y }, circle.radius, circle.radius), blackBrush.getRaw());
	}

	HRASSERT(renderTarget->EndDraw());

	ImGui::Render();
	return;
}
