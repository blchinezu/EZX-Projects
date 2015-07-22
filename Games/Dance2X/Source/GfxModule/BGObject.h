//! A class to define a Background object

#ifndef BGOBJECT_H
#define BGOBJECT_H


namespace GfxModule
{
	// Forward declare classes
	class SurfaceWrapper;

	class BGObject
	{
	private:
        //! The X position of the layer
		int _XPosition;

		//! The Y position of the layer
		int _YPosition;

		//! Is the layer visible or not
		bool _Visibility;

        //! Pointer to a surface wrapper used to hold the layer
		SurfaceWrapper *_SurfaceWrapper;

	public:
		//! Standard Constructor. NULLs the pointer and sets default values for X and Y position
		BGObject(const int XPosition = 0, const int YPosition = 0, SurfaceWrapper *ASurfaceWrapper = 0, bool Visibility = true) :
			_XPosition(XPosition),
			_YPosition(YPosition),
			_Visibility(Visibility),
			_SurfaceWrapper(ASurfaceWrapper)
		{
		}

		//! Standard Destructor. Does Nothing
		~BGObject(void)
		{
		}

		//! Set the Surface Wrapper
		void SetSurfaceWrapper(SurfaceWrapper *ASurfaceWrapper)
		{
            _SurfaceWrapper = ASurfaceWrapper;
        }

		//! Get the Surface Wrapper
		const SurfaceWrapper * const GetSurfaceWrapper(void) const
		{
            return _SurfaceWrapper;
        }

        //! Set the XY Position of the background
        void SetXYPosition (const int NewXPosition, const int NewYPosition)
        {
			_XPosition = NewXPosition;
			_YPosition = NewYPosition;
        }

		//! Set the X position of the background
		void SetXPosition (const int NewXPosition)
		{
            _XPosition = NewXPosition;
		}

		//! Set the Y position of the background
		void SetYPosition (const int NewYPosition)
		{
            _YPosition = NewYPosition;
		}

		//! Get the X position of the background
		const int GetXPosition(void) const
		{
            return _XPosition;
        }

        //! Get the Y position of the background
        const int GetYPosition(void) const
        {
            return _YPosition;
        }

        //! Move the background by X and Y
        void MoveBG(const int X, const int Y)
        {
            _XPosition += X;
            _YPosition += Y;
        }

        //! Set the layer visibility status
        void SetVisibility(const bool Visible)
        {
            _Visibility = Visible;
        }

        //! Switch the layer between visibility status
        void SwitchVisibility(void)
        {
            _Visibility = !_Visibility;
        }

        //! Scale the surface to a new width, height and filter use
        void ScaleBG(const unsigned int Width, const unsigned int Height);

        //! Scale the surface to a new width, height and filter use
        void ScaleBG(const float ScaleFactor);
	};
}

#endif // BGOBJECT_H

/*
History
=======
2006-06-25: Added support for scaling
2006-06-17: Added a function to clear the background object.
2006-06-16: Moved the XPosition, YPosition and Visibility to private and now have accessor functions
2006-06-15: Moved the class out of BGManager to here.
*/
