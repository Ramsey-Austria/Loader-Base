#pragma once

//standart
#include <Windows.h>
#include <string>
#include <iostream>
#include <dwmapi.h>

//d3d11
#include <d3d11.h>
ID3D11Device* device{ 0 };
ID3D11DeviceContext* devicecontext{ 0 };
IDXGISwapChain* swapchain{ 0 };
ID3D11RenderTargetView* rendertargetview{ 0 };
D3D_FEATURE_LEVEL level{};

//ImGui
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <imgui_internal.h>

//Protection
#include <Encryption.h>
#include <utils.h>


