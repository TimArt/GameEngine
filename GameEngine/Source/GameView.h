//
//  GameView.h
//  GameEngine
//
//  Created by Tim Arterbury on 8/26/17.
//
//

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "GameHUD.h"
#include "GameModel.h"

/** Represents the view of any game being rendered.
    It includes an OpenGL Renderer to render either 2D or 3D graphics and a
    GameHUD component to render elements over the top of the OpenGL Renderer.
 */
class GameView :    public Component,
                    private OpenGLRenderer
{
    
public:
    GameView()
    {
        // Sets the OpenGL version to 3.2
        // This is very important, if this is not included, new shader syntax
        // will cause a compiler error.
        openGLContext.setOpenGLVersionRequired (OpenGLContext::OpenGLVersion::openGL3_2);
        
        openGLContext.setRenderer(this);
        openGLContext.attachTo(*this);
        
        addAndMakeVisible(gameHUD);
        
        // Setup GUI Overlay Label: Status of Shaders, compiler errors, etc.
        addAndMakeVisible (statusLabel);
        statusLabel.setJustificationType (Justification::topLeft);
        statusLabel.setFont (Font (14.0f));
        statusLabel.toBack();
        
        // GameView Variables
        isEnabled = false;
        
        setOpaque(true);
    }
    
    ~GameView()
    {
        // Turn off OpenGL
		
        openGLContext.setContinuousRepainting (false);
        openGLContext.detach();
		texture.release();
		
    }
    
    /** Enables or disables the OpenGL layer of GameView. Enabling continuously
        renders and repaints with OpenGL. Disabling will set a solid colour fill
        for the component and the component will do no processing.
     */
    void setOpenGLEnabled (bool enabled)
    {
        if (enabled)
        {
            openGLContext.setContinuousRepainting(true);
            isEnabled = true;
        }
        else
        {
            openGLContext.setContinuousRepainting(false);
            isEnabled = false;
        }
    }
    
    // OpenGL Callbacks ========================================================
    void newOpenGLContextCreated() override
    {
		//Load Textures
		//texture.bind();

        // Setup Shaders
        createShaders();

		//String filePath = File::getCurrentWorkingDirectory().getFullPathName() + "\\textures\\flower.jpg";
		//File f = filePath;

		//Image textureImage = ImageFileFormat::loadFrom(f); //ImageCache::getFromMemory (TEXTURE_DATA);
														   // Image must have height and width equal to a power of 2 pixels to be more efficient
														   // when used with older GPU architectures
		//if (!(isPowerOfTwo(textureImage.getWidth()) && isPowerOfTwo(textureImage.getHeight())))
		//	textureImage = textureImage.rescaled(jmin(1024, nextPowerOfTwo(textureImage.getWidth())),
		//		jmin(1024, nextPowerOfTwo(textureImage.getHeight())));
		// Use that image as a 2-D texture for the object that will be painted
		//texture.loadImage(textureImage);

        
        // Setup Buffer Objects
        openGLContext.extensions.glGenBuffers (1, &VBO); // Vertex Buffer Object
        openGLContext.extensions.glGenBuffers (1, &EBO); // Element Buffer Object
    }
    
    void openGLContextClosing() override
    {
        shader = nullptr;
        uniforms = nullptr;
    }
    
    void renderOpenGL() override
    {
		//wait for Core Engine to say go
		renderWaitable->wait();
		
        jassert (OpenGLHelpers::isContextActive());
        
        // Setup Viewport
        const float renderingScale = (float) openGLContext.getRenderingScale();
        glViewport (0, 0, roundToInt (renderingScale * getWidth()), roundToInt (renderingScale * getHeight()));
    
        // Set background Color
        OpenGLHelpers::clear (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
        
        // Enable Alpha Blending
        glEnable (GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthFunc(GL_LESS);      // Do not display back pixels
		glEnable(GL_DEPTH_TEST);
		openGLContext.extensions.glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//texture.bind();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        
        // Use Shader Program that's been defined
        shader->use();

		if (uniforms->demoTexture != nullptr) {
			uniforms->demoTexture->set((GLint)0);
		}

		//uniforms->textureType->set((GLfloat) 2.0);

		// Modify the uniform (global) variable projectionMatrix that will be used by the GPU when executing the shaders
		if (uniforms->projectionMatrix != nullptr)
			// Update the projection matrix with the values given, 1 matrix, do not transpose
			uniforms->projectionMatrix->setMatrix4(getProjectionMatrix().mat, 1, false);

		// Modify the uniform (global) variable viewMatrix that will be used by the GPU when executing the shaders
		if (uniforms->viewMatrix != nullptr)
			// Update the view matrix with the values given, 1 matrix, do not transpose
			uniforms->viewMatrix->setMatrix4(getViewMatrix().mat, 1, false);
		
		Array<GLfloat> floatArr = (*gameModelContainer)->drawableObjects.getFirst()->getVertices();
		GLfloat* vertices = floatArr.getRawDataPointer(); //gameModel->drawableObjects.getFirst()->getVertices().getRawDataPointer();

        // Define Vertices for a Square (the view plane)
       /* GLfloat vertices[] = {
            1.0f,   1.0f,  0.0f,  // Top Right
            1.0f,  -1.0f,  0.0f,  // Bottom Right
            -1.0f, -1.0f,  0.0f,  // Bottom Left
            -1.0f,  1.0f,  0.0f   // Top Left
        };*/
        // Define Which Vertex Indexes Make the Square
        GLuint indices[] = {  // Note that we start from 0!
            0, 1, 3,   // First Triangle
            1, 2, 3    // Second Triangle
        };

        // VBO (Vertex Buffer Object) - Bind and Write to Buffer
        openGLContext.extensions.glBindBuffer (GL_ARRAY_BUFFER, VBO);
        openGLContext.extensions.glBufferData (GL_ARRAY_BUFFER, sizeof(GLfloat)*12, vertices, GL_STREAM_DRAW);
                                                                                        // GL_DYNAMIC_DRAW or GL_STREAM_DRAW
                                                                                        // We may want GL_DYNAMIC_DRAW since it is for
                                                                                        // vertex data that will be changing alot.
        
        // EBO (Element Buffer Object) - Bind and Write to Buffer
        openGLContext.extensions.glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, EBO);
        openGLContext.extensions.glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STREAM_DRAW);
                                                                                // GL_DYNAMIC_DRAW or GL_STREAM_DRAW
                                                                                // We may want GL_DYNAMIC_DRAW since it is for
                                                                                // vertex data that will be changing alot.
       
        // Setup Vertex Attributes
        openGLContext.extensions.glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        openGLContext.extensions.glEnableVertexAttribArray (0);
        
		
        // Draw Vertices
        glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // For EBO's (Element Buffer Objects) (Indices)
        //glDrawArrays (GL_TRIANGLES, 0, 6); // For just VBO's (Vertex Buffer Objects)
        
        
        // Reset the element buffers so child Components draw correctly
        openGLContext.extensions.glBindBuffer (GL_ARRAY_BUFFER, 0);
        openGLContext.extensions.glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
        //openGLContext.extensions.glBindVertexArray(0);

		//Tell Core Engine I am donezo
		coreWaitable->signal();
		
    }
    
    
    // JUCE Callbacks ==========================================================
    void resized() override
    {
        gameHUD.setBounds(getLocalBounds());
        statusLabel.setBounds (getLocalBounds().reduced (4).removeFromTop (75));
    }
    
    void paint(Graphics & g) override
    {}

	void setGameModel(GameModel** modelContainer) {
		gameModelContainer = modelContainer;
	}

	void setCoreWaitable(WaitableEvent* waitable) {
		coreWaitable = waitable;
	}

	void setLoopWaitable(WaitableEvent* waitable) {
		renderWaitable = waitable;
		
	}
    
    
private:
    
    //==========================================================================
    // OpenGL Shader Functions

    /** Calculates and returns the Projection Matrix.
     */
    Matrix3D<float> getProjectionMatrix() const
    {
        float w = 1.0f / (0.5f + 0.1f);
        float h = w * getLocalBounds().toFloat().getAspectRatio (false);
        return Matrix3D<float>::fromFrustum (-w, w, -h, h, 4.0f, 30.0f);
    }
    
    /** Calculates and returns the View Matrix.
     */
    Matrix3D<float> getViewMatrix() const
    {
        Matrix3D<float> viewMatrix (Vector3D<float> (0.0f, 0.0f, -10.0f));
       // Matrix3D<float> rotationMatrix = draggableOrientation.getRotationMatrix();
        
		return viewMatrix;//rotationMatrix * viewMatrix;
    }
    
    /** Loads the OpenGL Shaders and sets up the whole ShaderProgram
     */
    void createShaders()
    {
        /*vertexShader =
        "#version 330 core\n"
        "layout (location = 0) in vec2 position;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    gl_Position = vec4(position, 0.0f, 1.0f);\n"
        "}\n";*/

		vertexShader = 
			"#version 330 core\n"
			"attribute vec4 position;\n"
			"attribute vec4 sourceColour;\n"
			"attribute vec2 textureCoordIn;\n"
			"\n"
			"uniform mat4 projectionMatrix;\n"
			"uniform mat4 viewMatrix;\n"
			"\n"
			"varying vec4 destinationColour;\n"
			"varying vec2 textureCoordOut;\n"
			"\n"
			"void main()\n"
			"{\n"
			"    destinationColour = sourceColour;\n"
			"    textureCoordOut = textureCoordIn;\n"
			"    gl_Position = projectionMatrix * viewMatrix * position;\n"
			"}\n";
        
        // Base Fragment-Shader paints the object green.
       /* fragmentShader =
        "#version 330 core\n"
        "out vec4 color;\n"
        "void main()\n"
        "{\n"
        "    color = vec4 (0.0f, 0.50f, 0.70f, 1.0f);\n"
        "}\n";*/

		fragmentShader =
			"#version 330 core\n"
#if JUCE_OPENGL_ES
			"precision lowp float;\n"
			"varying lowp vec4 destinationColour;\n"
			"varying lowp vec2 textureCoordOut;\n"
#else
			"varying vec4 destinationColour;\n"
			"varying vec2 textureCoordOut;\n"
#endif
			"\n"
			"uniform sampler2D demoTexture;\n"
			"uniform float textureType;\n"
			"\n"
			"void main()\n"
			"{\n"
			"    if (textureType < 0.1)\n"
			"        gl_FragColor = destinationColour;\n"
			"    else if (textureType < 2.1)\n"
			"     gl_FragColor = texture2D (demoTexture, textureCoordOut);\n"
			"}\n";
        
        
        ScopedPointer<OpenGLShaderProgram> newShader (new OpenGLShaderProgram (openGLContext));
        String statusText;
        
        if (newShader->addVertexShader ((vertexShader))
            && newShader->addFragmentShader ((fragmentShader))
            && newShader->link())
        {
            uniforms = nullptr;
            
            shader = newShader;
            shader->use();
            
            uniforms   = new Uniforms (openGLContext, *shader);
            
            statusText = "GLSL: v" + String (OpenGLShaderProgram::getLanguageVersion(), 2);
			String tmp = "\\textures\\flower.jpg";
			//statusText = File::getCurrentWorkingDirectory().getFullPathName();
			//statusText += tmp;
        }
        else
        {
            statusText = newShader->getLastError();
        }
        
        statusLabel.setText (statusText, dontSendNotification);
    }
    
    //==============================================================================
    // This class manages the uniform values that the shaders use.
    struct Uniforms
    {
        Uniforms (OpenGLContext& openGLContext, OpenGLShaderProgram& shaderProgram)
        {
            projectionMatrix = createUniform (openGLContext, shaderProgram, "projectionMatrix");
            viewMatrix       = createUniform (openGLContext, shaderProgram, "viewMatrix");
			demoTexture = createUniform(openGLContext, shaderProgram, "demoTexture");
			textureType = createUniform(openGLContext, shaderProgram, "textureType");
        }
        
        ScopedPointer<OpenGLShaderProgram::Uniform> projectionMatrix, viewMatrix, demoTexture, textureType;
        //ScopedPointer<OpenGLShaderProgram::Uniform> lightPosition;
        
        private:
        static OpenGLShaderProgram::Uniform* createUniform (OpenGLContext& openGLContext,
                                                            OpenGLShaderProgram& shaderProgram,
                                                            const char* uniformName)
        {
            if (openGLContext.extensions.glGetUniformLocation (shaderProgram.getProgramID(), uniformName) < 0)
            return nullptr;
            
            return new OpenGLShaderProgram::Uniform (shaderProgram, uniformName);
        }
    };
    
    
    bool isEnabled;
    
    // OpenGL Rendering
    OpenGLContext openGLContext;
    ScopedPointer<OpenGLShaderProgram> shader;
    ScopedPointer<Uniforms> uniforms;
    GLuint VBO;
    GLuint VAO;
    GLuint EBO;
    
    const char* vertexShader;
    const char* fragmentShader;

	//Textures Testing
	OpenGLTexture texture;
    
    // JUCE Components
    GameHUD gameHUD;
    
    // DEBUGGING
    Label statusLabel;

	//GameModel
	GameModel** gameModelContainer;
	WaitableEvent* renderWaitable;
	WaitableEvent* coreWaitable;

	int i = 0;
    
};
