#include <gl/gl.h>
#include <Eigen/Dense>
//-----get the ray from a scree point to OpenGL viewing volume------
void GetSelectionRay(int mouse_x, int mouse_y,GLdouble *near_point,GLdouble *far_point) {
    // 获取 Model-View、Projection 矩阵 & 获取Viewport视区
    GLdouble    modelview[16];
    GLdouble    projection[16];
    GLint       viewport[4];
    glGetDoublev (GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev (GL_PROJECTION_MATRIX, projection);
    glGetIntegerv (GL_VIEWPORT, viewport);

	mouse_y = 	viewport[3] - mouse_y;
    // get the intersection of ray with near plane
    gluUnProject( (GLdouble) mouse_x, (GLdouble) mouse_y, 0.0, 
                    modelview, projection, viewport, 
                    &near_point[0], &near_point[1], &near_point[2]); 
	// get the intersection of ray with far plane
    gluUnProject( (GLdouble) mouse_x, (GLdouble) mouse_y, 1.0, 
                    modelview, projection, viewport, 
                    &far_point[0], &far_point[1], &far_point[2]); 
}

//----find the nearest point to the ray from screen point (x,y) in 3d point set V------
int DoSelect(const Eigen::MatrixXd &V,int x, int y)
{	
	Eigen::Vector3d nearP, farP;
	GetSelectionRay(x, y, nearP.data(), farP.data());
	Eigen::Vector3d vec(farP[0] - nearP[0], farP[1] - nearP[1], farP[2] - nearP[2]);

	vec.normalize();
	double min = 1.e9; 
	int index = -1;

	for(int i = 0;i<V.rows();i++){
		Eigen::Vector3d VP = V.row(i).transpose() - nearP;
		Eigen::Vector3d N = VP.cross(V);
	
		double d = N.norm();
		if(d<min){
			min = d;index = i;
		}
	}	
	return index;	
}
