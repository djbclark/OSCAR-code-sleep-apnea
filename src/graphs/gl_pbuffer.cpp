
#include "gl_pbuffer.h"

#include <wx/utils.h>

pBuffer::pBuffer()
{
}
pBuffer::pBuffer(int width, int height)
:m_width(width),m_height(height)
{
}
pBuffer::~pBuffer()
{
}


#if defined(__WXMSW__)

#if !defined(wglGetExtensionsStringARB)
    PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB = NULL;

    // WGL_ARB_pbuffer
    PFNWGLCREATEPBUFFERARBPROC    wglCreatePbufferARB    = NULL;
    PFNWGLGETPBUFFERDCARBPROC     wglGetPbufferDCARB     = NULL;
    PFNWGLRELEASEPBUFFERDCARBPROC wglReleasePbufferDCARB = NULL;
    PFNWGLDESTROYPBUFFERARBPROC   wglDestroyPbufferARB   = NULL;
    PFNWGLQUERYPBUFFERARBPROC     wglQueryPbufferARB     = NULL;

    // WGL_ARB_pixel_format
    PFNWGLGETPIXELFORMATATTRIBIVARBPROC wglGetPixelFormatAttribivARB = NULL;
    PFNWGLGETPIXELFORMATATTRIBFVARBPROC wglGetPixelFormatAttribfvARB = NULL;
    PFNWGLCHOOSEPIXELFORMATARBPROC      wglChoosePixelFormatARB      = NULL;
#endif


pBufferWGL::pBufferWGL(int width, int height)
:m_texture(0)
{

    hGlRc=0;
    hBuffer=0;
    hdc=0;
    saveHdc=0;
    saveHglrc=0;

    if (!InitGLStuff()) {
        throw GLException(wxT("Could not Init GL Stuff"));
    }

    // Adjust to square power of 2
    int ms=MAX(width,height);
    int j;
    for (int i=4;i<32;i++) {  // min size 16x16.. probably a usless size.
        j=1 << i;
        if (j >= ms) break;
    }
    j <<= 2;
    //assert (j>=ms); // I seriously doubt this will ever happen ;)

    // WGL only supports square pBuffers
    m_width=j;
    m_height=j;

    wxLogDebug(wxString::Format(wxT("Adjusting pBuffer width and height to %ix%i"),j,j));

    // Create Texture
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, 4,  m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    // Get the context to work with (must be valid, will die otherwise)
    saveHdc = wglGetCurrentDC();
    saveHglrc = wglGetCurrentContext();

    int pixelFormats;
	int intAttrs[32] ={
        WGL_RED_BITS_ARB,8,
        WGL_GREEN_BITS_ARB,8,
        WGL_BLUE_BITS_ARB,8,
        WGL_ALPHA_BITS_ARB,8,
        WGL_DRAW_TO_PBUFFER_ARB, GL_TRUE,
        WGL_BIND_TO_TEXTURE_RGBA_ARB, GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB,GL_TRUE,
        WGL_ACCELERATION_ARB,WGL_FULL_ACCELERATION_ARB,
        WGL_DOUBLE_BUFFER_ARB,GL_FALSE,
        0
    }; // 0 terminate the list

	unsigned int numFormats = 0;

    if (!wglChoosePixelFormatARB) {
        throw GLException(wxT("No wglChoosePixelFormatARB available"));
    }
	if (!wglChoosePixelFormatARB( saveHdc, intAttrs, NULL, 1, &pixelFormats, &numFormats)) {
        throw GLException(wxT("WGL: pbuffer creation error: Couldn't find a suitable pixel format."));
	}
    if (numFormats==0) {
        throw GLException(wxT("WGL: pbuffer creation error: numFormats==0"));
    }
    const int attributes[]= {
        WGL_TEXTURE_FORMAT_ARB,
        WGL_TEXTURE_RGBA_ARB, // p-buffer will have RBA texture format
        WGL_TEXTURE_TARGET_ARB,
        WGL_TEXTURE_2D_ARB,
        0
    }; // Of texture target will be GL_TEXTURE_2D

    //wglCreatePbufferARB(hDC, pixelFormats, pbwidth, pbheight, attributes);
	hBuffer=wglCreatePbufferARB(saveHdc, pixelFormats, m_width, m_height, attributes );
	if (!hBuffer) {
	    throw GLException(wxT("wglCreatePbufferARB failure"));
	}

    hdc=wglGetPbufferDCARB( hBuffer );
	if (!hdc) {
	    throw GLException(wxT("wglGetPbufferDCARB failure"));
	}

    hGlRc=wglCreateContext(hdc);
	if (!hGlRc) {
	    throw GLException(wxT("wglCreateContext failure"));
	}

        //printf("PBuffer size: %d x %d\n",w,h);
    int w,h;
	wglQueryPbufferARB(hBuffer, WGL_PBUFFER_WIDTH_ARB, &w);
	wglQueryPbufferARB(hBuffer, WGL_PBUFFER_HEIGHT_ARB, &h);

	// compare w & h to m_width & m_height.

 /*   wglMakeCurrent(hdc,hGlRc);

    glEnable(GL_TEXTURE_2D);		      // Enable Texture Mapping
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); // enable transparency

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, height, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT); */


    // switch back to the screen context
	wglMakeCurrent(saveHdc, saveHglrc);
    // So we can share the main context
    wglShareLists(saveHglrc, hGlRc);

    // Jump back to pBuffer ready for rendering
    //wglMakeCurrent(hdc, hGlRc);
}
pBufferWGL::~pBufferWGL()
{
    if (hGlRc) wglDeleteContext(hGlRc);
    if (hBuffer) wglReleasePbufferDCARB(hBuffer, hdc);
    if (hBuffer) wglDestroyPbufferARB(hBuffer);
}
void pBufferWGL::UseBuffer(bool b)
{
    if (b) {
        wglMakeCurrent(hdc, hGlRc);
    } else {
        wglMakeCurrent(saveHdc, saveHglrc);
    }
}

bool pBufferWGL::InitGLStuff()
{

    // Technically don't need this wrangling with glewInit being actually called now....
    //wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB");
	char *ext = NULL;

	if (wglGetExtensionsStringARB)
		ext = (char*)wglGetExtensionsStringARB( wglGetCurrentDC() );
	else {
	    wxLogError(wxT("Unable to get address for wglGetExtensionsStringARB!"));
	    return false;
	}

	if (strstr(ext, "WGL_ARB_pbuffer" ) == NULL) {
        wxLogError(wxT("WGL_ARB_pbuffer extension was not found"));
        return false;
	} else {
		//wglCreatePbufferARB    = (PFNWGLCREATEPBUFFERARBPROC)wglGetProcAddress("wglCreatePbufferARB");
		//wglGetPbufferDCARB     = (PFNWGLGETPBUFFERDCARBPROC)wglGetProcAddress("wglGetPbufferDCARB");
		//wglReleasePbufferDCARB = (PFNWGLRELEASEPBUFFERDCARBPROC)wglGetProcAddress("wglReleasePbufferDCARB");
		//wglDestroyPbufferARB   = (PFNWGLDESTROYPBUFFERARBPROC)wglGetProcAddress("wglDestroyPbufferARB");
		//wglQueryPbufferARB     = (PFNWGLQUERYPBUFFERARBPROC)wglGetProcAddress("wglQueryPbufferARB");

		if (!wglCreatePbufferARB || !wglGetPbufferDCARB || !wglReleasePbufferDCARB ||
			!wglDestroyPbufferARB || !wglQueryPbufferARB) {
            wxLogError(wxT("One or more WGL_ARB_pbuffer functions were not found"));
            return false;
		}
	}

	// WGL_ARB_pixel_format
	if (strstr( ext, "WGL_ARB_pixel_format" ) == NULL) {
	    wxLogError(wxT("WGL_ARB_pixel_format extension was not found"));
		return false;
	} else {
		wglGetPixelFormatAttribivARB = (PFNWGLGETPIXELFORMATATTRIBIVARBPROC)wglGetProcAddress("wglGetPixelFormatAttribivARB");
		wglGetPixelFormatAttribfvARB = (PFNWGLGETPIXELFORMATATTRIBFVARBPROC)wglGetProcAddress("wglGetPixelFormatAttribfvARB");
		wglChoosePixelFormatARB      = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");

		if (!wglGetExtensionsStringARB || !wglCreatePbufferARB || !wglGetPbufferDCARB) {
			wxLogError(wxT("One or more WGL_ARB_pixel_format functions were not found"));
			return false;
		}
	}


	return true;

}

#elif defined(__UNIX__) && !defined(__WXMAC__)

GLXContext real_shared_context=NULL;


pBufferGLX::pBufferGLX(int width, int height)
:pBuffer()
{
    m_width=width;
    m_height=height;

    int attrib[]={
        GLX_PBUFFER_WIDTH,m_width,
        GLX_PBUFFER_HEIGHT,m_height,
        GLX_PRESERVED_CONTENTS, True
    };

    pBuffer=0;
    m_context=0,m_shared=0;
    int ret;
    display=NULL;
    GLXFBConfig *fbc=NULL;

/*#if wxCHECK_VERSION(2,9,0)
    display=wxGetX11Display();
    fbc = GetGLXFBConfig(); // wxGLCanvas call
    fbc = &fbc[0];
    pBuffer=glXCreatePbuffer(display, fbc[0], attrib );

#else */
    display=(Display *)wxGetDisplay();
    int doubleBufferAttributess[] = {
        GLX_DRAWABLE_TYPE, GLX_PBUFFER_BIT,
        GLX_RENDER_TYPE, GLX_RGBA_BIT,
        GLX_DOUBLEBUFFER, True,
        GLX_RED_SIZE, 8,
        GLX_GREEN_SIZE, 8,
        GLX_BLUE_SIZE, 8,
        None
    };
    fbc=glXChooseFBConfig(display, DefaultScreen(display), doubleBufferAttributess, &ret);
    pBuffer=glXCreatePbuffer(display, *fbc, attrib );
//#endif

    if (pBuffer == 0) {
        wxLogError(wxT("pBuffer not availble"));
    }



    // This function is not in WX sourcecode yet :(
    //shared=shared_context->GetGLXContext();

    if (real_shared_context) m_shared=real_shared_context; // Only available after redraw.. :(
    else {
        // First render screws up unless we do this..
        m_shared=m_context= glXCreateNewContext(display,*fbc,GLX_RGBA_TYPE, NULL, True);
    }

    if (m_shared == 0) {
        wxLogError(wxT("Context not availble"));
    }

//#if !wxCHECK_VERSION(2,9,0)
    XFree(fbc);
//#endif

    glXMakeCurrent(display,pBuffer,m_shared);

    //UseBuffer(true);
}
pBufferGLX::~pBufferGLX()
{
    if (m_context) glXDestroyContext(display,m_context); // Destroy the context only if we created it..
    if (pBuffer) glXDestroyPbuffer(display, pBuffer);
}
void pBufferGLX::UseBuffer(bool b)
{
    if (b) {
        if (glXMakeCurrent(display,pBuffer,m_shared)!=True) {
            wxLogError(wxT("Couldn't make pBuffer current"));
        }
    } else {

        // to be honest.. i'm not sure yet.. wx stupidly keeps the needed variables private
    }
}
#elif defined(__DARWIN__) || defined(__WXMAC__)


#endif
