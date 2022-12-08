#include "includes.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd , UINT msg , WPARAM wParam , LPARAM lParam );

LRESULT CALLBACK windowproc( HWND windowhwnd , UINT msg , WPARAM wparam , LPARAM lparam )
{
	if ( ImGui_ImplWin32_WndProcHandler( windowhwnd , msg , wparam , lparam ) )
	{
		return 0L;
	}

	if ( msg == WM_DESTROY )
	{
		PostQuitMessage( 0 );
		return 0L;
	}

	return DefWindowProc( windowhwnd , msg , wparam , lparam );
}


int APIENTRY WinMain( HINSTANCE instance , HINSTANCE , PSTR , INT cmd )
{
	auto title = utils::randomstring( 15 );
	auto classname = utils::randomstring( 13 );

	WNDCLASSEXW wc {};
	wc.cbSize = sizeof( WNDCLASSEXW );
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = windowproc;
	wc.hInstance = instance;
	wc.lpszClassName = (LPCWSTR) classname.c_str( );

	RegisterClassExW( &wc );

	const HWND window = CreateWindowExW( 0 , wc.lpszClassName , (LPCWSTR) title.c_str( ) , WS_SYSMENU , 0 , 0 , 700 , 400 , nullptr , nullptr , wc.hInstance , nullptr );

	SetLayeredWindowAttributes( window , RGB( 13 , 13 , 13 ) , BYTE( 255 ) , LWA_ALPHA );

	DXGI_SWAP_CHAIN_DESC sd {};
	sd.BufferDesc.RefreshRate.Numerator = 60U;
	sd.BufferDesc.RefreshRate.Denominator = 1U;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.SampleDesc.Count = 1U;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2U;
	sd.OutputWindow = window;
	sd.Windowed = 1;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	constexpr D3D_FEATURE_LEVEL levels [ 2 ]
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_0
	};

	D3D11CreateDeviceAndSwapChain( nullptr , D3D_DRIVER_TYPE_HARDWARE , nullptr , 0U , levels , 2U , D3D11_SDK_VERSION , &sd , &swapchain , &device , &level , &devicecontext );

	ID3D11Texture2D* backbuffer { 0 };
	swapchain->GetBuffer( 0U , IID_PPV_ARGS( &backbuffer ) );

	if ( backbuffer )
	{
		device->CreateRenderTargetView( backbuffer , 0 , &rendertargetview );
		backbuffer->Release( );
	}
	else
	{
		return 1;
	}

	ShowWindow( window , cmd );
	UpdateWindow( window );

	ImGui::CreateContext( );

	ImGuiIO& io = ImGui::GetIO( );
	io.Fonts->AddFontFromFileTTF( Encrypt( "C:\\Windows\\Fonts\\Verdana.ttf" ) , 14 ); // Font at here if you want some

	ImGui::StyleColorsDark( );

	ImGui_ImplWin32_Init( window );
	ImGui_ImplDX11_Init( device , devicecontext );

	bool run = 1;
	while ( run )
	{
		MSG msg;
		while ( PeekMessage( &msg , nullptr , 0U , 0U , PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
			if ( msg.message == WM_QUIT )
			{
				run = false;
			}
		}

		if ( !run )
		{
			break;
		}

		ImGui_ImplDX11_NewFrame( );
		ImGui_ImplWin32_NewFrame();

		ImGui::NewFrame( );

		//Styles
		{
			ImGuiStyle& style = ImGui::GetStyle( );
			style.Colors [ ImGuiCol_WindowBg ] = ImColor( 23 , 23 , 23 );
		}

		ImGui::Begin( title.c_str( ) , nullptr , ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar );
		{
			ImGui::SetWindowSize( { 700, 400 } );
			ImGui::SetWindowPos( { 0, 0 } );
			ImGui::SetCursorPos( { 700 / 2 - 100, 100 } );
			ImGui::Text( Encrypt( "Made by Ramsey#5866" ) );

			ImGui::SetCursorPos( { 10, 120 } );
			ImGui::Text( Encrypt( "Info: It has a Encryption, It changes the name in chinese symbols + the window class name, you can use it for " ) );
			ImGui::SetCursorPos( { 10, 140 } );
			ImGui::Text( Encrypt( "external cheats too" ) );
		}
		ImGui::End( );



		ImGui::Render( );

		constexpr float color [ 4 ] { 0.f, 0.f, 0.f, 0.f };
		devicecontext->OMSetRenderTargets( 1U , &rendertargetview , nullptr );
		devicecontext->ClearRenderTargetView( rendertargetview , color );

		ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData( ) );

		swapchain->Present( 1U , 0U );

	}

	ImGui_ImplDX11_Shutdown( );
	ImGui_ImplWin32_Shutdown( );

	ImGui::DestroyContext( );

	if ( swapchain )
		swapchain->Release( );

	if ( devicecontext )
		devicecontext->Release( );

	if ( device )
		device->Release( );

	if ( rendertargetview )
		rendertargetview->Release( );

	DestroyWindow( window );
	UnregisterClassW( wc.lpszClassName , instance );

	return 0;
}