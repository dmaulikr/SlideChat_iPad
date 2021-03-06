//
//  CGPointUtils.h
//  SlideTime
//
//  Created by Frederic Jacobs on 11/8/12.


//  SlideChat is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// SlideChat Source Code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details. See <http://www.gnu.org/licenses/>.


#import <UIKit/UIKit.h>
#import <math.h>

CGFloat distanceBetweenPoints (CGPoint first, CGPoint second);
CGFloat angleBetweenPoints(CGPoint first, CGPoint second);

typedef struct 
{
	CGPoint point1;
	CGPoint point2;
} CGLine;

typedef struct 
{
	CGPoint center;
	CGFloat radius;
} CGCircle;

typedef struct 
{
	CGLine line1;
	CGLine line2;
	
	CGPoint center;
} CGX;

typedef struct
{
	CGPoint center;
	CGFloat width;
	CGFloat height;
} CGSquare;

CGCircle CGMakeCircle(CGPoint center, CGFloat radius);
CGX CGMakeX(CGLine line1, CGLine line2, CGPoint center);
CGLine CGMakeLine(CGPoint point1, CGPoint point2);
CGSquare CGMakeSquare(CGPoint center, CGFloat width, CGFloat height);

CGFloat angleBetweenLines(CGLine line1, CGLine line2);
CGFloat distanceBetweenLines(CGLine line1, CGLine line2);
BOOL CGCircleCollision(CGCircle circle1, CGCircle circle2);