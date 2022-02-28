#include <cmath>

struct mat4 {
	float a, b, c, d,  e, f, g, h,  i, j, k, l,  m, n, o, p;
	mat4(float a,float b,float c,float d, float e,float f,float g,float h, float i,float j,float k,float l, float m,float n,float o,float p):
	a(a),b(b),c(c),d(d),e(e),f(f),g(g),h(h),i(i),j(j),k(k),l(l),m(m),n(n),o(o),p(p) {};
	mat4(): a(0), b(0), c(0), d(0), e(0), f(0), g(0), h(0), i(0), j(0), k(0), l(0), m(0), n(0), o(0), p(0) {};
	
	mat4 operator*(mat4 w) {
		return mat4(
			a*w.a + b*w.e + c*w.i + d*w.m,
			a*w.b + b*w.f + c*w.j + d*w.n,
			a*w.c + b*w.g + c*w.k + d*w.o,
			a*w.d + b*w.h + c*w.l + d*w.p,
			
			e*w.a + f*w.e + g*w.i + h*w.m,
			e*w.b + f*w.f + g*w.j + h*w.n,
			e*w.c + f*w.g + g*w.k + h*w.o,
			e*w.d + f*w.h + g*w.l + h*w.p,
			
			i*w.a + j*w.e + k*w.i + l*w.m,
			i*w.b + j*w.f + k*w.j + l*w.n,
			i*w.c + j*w.g + k*w.k + l*w.o,
			i*w.d + j*w.h + k*w.l + l*w.p,
			
			m*w.a + n*w.e + o*w.i + p*w.m,
			m*w.b + n*w.f + o*w.j + p*w.n,
			m*w.c + n*w.g + o*w.k + p*w.o,
			m*w.d + n*w.h + o*w.l + p*w.p
		);
	}
};

struct mat3 {
	float a, b, c,  d, e, f,  g, h, i;
	mat3(float a,float b,float c,float d,float e,float f,float g,float h,float i):
	a(a),b(b),c(c),d(d),e(e),f(f),g(g),h(h),i(i) {};
	mat3(): a(0), b(0), c(0), d(0), e(0), f(0), g(0), h(0), i(0) {};
};

template<class T> struct vec2 {
	T x, y;
	
	vec2<T>(): x(0), y(0) {};
	vec2<T>(T _x): x(_x), y(_x) {};
	vec2<T>(T _x, T _y): x(_x), y(_y) {};
	vec2<T>(const vec2<T> &v) : x(T()), y(T()) { *this = v; }
	
	vec2<T> operator+(vec2<T> v) {
		return vec2<T>(x + v.x, y + v.y);
	}
	
	vec2<T> operator-(vec2<T> v) {
		return vec2<T>(x - v.x, y - v.y);
	}
	
	vec2<T> operator*(vec2<T> v) {
		return vec2<T>(x * v.x, y * v.y);
	}
	
	vec2<T> operator*(T v) {
		return vec2<T>(x * v, y * v);
	}
	
	vec2<T> operator/(vec2<T> v) {
		return vec2<T>(x / v.x, y / v.y);
	}
	
	vec2<T> operator/(T v) {
		return vec2<T>(x / v, y / v);
	}
};

template<class T> struct vec3 {
	T x, y, z;
	
	vec3<T>(): x(0), y(0), z(0) {};
	vec3<T>(T _x): x(_x), y(_x), z(_x) {};
	vec3<T>(T _x, T _y, T _z): x(_x), y(_y), z(_z) {};
	vec3<T>(const vec3<T> &v): x(v.x), y(v.y), z(v.z) {};
	
	vec3<T> operator+(vec3<T> v) {
		return vec3<T>(x + v.x, y + v.y, z + v.z);
	}
	
	vec3<T> operator-(vec3<T> v) {
		return vec3<T>(x - v.x, y - v.y, z - v.z);
	}
	
	vec3<T> operator*(vec3<T> v) {
		return vec3<T>(x * v.x, y * v.y, z * v.z);
	}
	
	vec3<T> operator*(T v) {
		return vec3<T>(x * v, y * v, z * v);
	}
	
	vec3<T> operator*(mat3 m) {
		return vec3<T>(
			x * m.a + y * m.b + z * m.c,
			x * m.d + y * m.e + z * m.f,
			x * m.g + y * m.h + z * m.i
		);
	}
	
	vec3<T> operator/(vec3<T> v) {
		return vec3<T>(x / v.x, y / v.y, z / v.z);
	}
	
	vec3<T> operator/(T v) {
		return vec3<T>(x / v, y / v, z / v);
	}
};

template<class T> struct vec4 {
	T x, y, z, w;
	
	vec4<T>(): x(0), y(0), z(0), w(0) {};
	vec4<T>(T _x): x(_x), y(_x), z(_x), w(_x) {};
	vec4<T>(T _x, T _y, T _z, T _w): x(_x), y(_y), z(_z), w(_w) {};
	vec4<T>(const vec4<T> &v): x(v.x), y(v.y), z(v.z), w(v.w) {};
	
	vec4<T> operator+(vec4<T> v) {
		return vec4<T>(x + v.x, y + v.y, z + v.z, w + v.w);
	}
	
	vec4<T> operator-(vec4<T> v) {
		return vec4<T>(x - v.x, y - v.y, z - v.z, w - v.w);
	}
	
	vec4<T> operator*(vec4<T> v) {
		return vec4<T>(x * v.x, y * v.y, z * v.z, w * v.w);
	}
	
	vec4<T> operator*(T v) {
		return vec4<T>(x * v, y * v, z * v, w * v);
	}
	
	vec4<T> operator*(mat4 m) {
		return vec4<T>(
			x * m.a + y * m.b + z * m.c + w * m.d,
			x * m.e + y * m.f + z * m.g + w * m.h,
			x * m.i + y * m.j + z * m.k + w * m.l,
			x * m.m + y * m.n + z * m.o + w * m.p
		);
	}
	
	vec4<T> operator/(vec4<T> v) {
		return vec4<T>(x / v.x, y / v.y, z / v.z, w / v.w);
	}
	
	vec4<T> operator/(T v) {
		return vec4<T>(x / v, y / v, z / v, w / v);
	}
};


typedef vec2<int> vec2i;
typedef vec2<float> vec2f;
typedef vec3<int> vec3i;
typedef vec3<float> vec3f;
typedef vec4<int> vec4i;
typedef vec4<float> vec4f;


float length(vec3f v) {
	return powf((v.x * v.x + v.y * v.y + v.z * v.z), 0.5);
}

vec3f normalize(vec3f v) {
	return v / length(v);
}

vec3f cross(vec3f a, vec3f b) {
	vec3f c;
	c.x = a.y * b.z - a.z * b.y;
	c.y = a.z * b.x - a.x * b.z;
	c.z = a.x * b.y - a.y * b.x;
	return c;
}

float dot(vec3f a, vec3f b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

template<class T>
T mix(T a, T b, float k) {
	return a * k + b * (1. - k);
}

template<class T>
T sign(T a) {
	if (a == 0) return 0;
	if (a < 0) return -1;
	return 1;
}

vec3f sign(vec3f a) {
	vec3f r(0);
	if (a.x < 0) r.x = -1; if (a.x > 0) r.x = 1;
	if (a.y < 0) r.y = -1; if (a.y > 0) r.y = 1;
	if (a.z < 0) r.x = -1; if (a.z > 0) r.z = 1;
	return r;
}

vec3f step(vec3f e, vec3f x) {
	vec3f r(0);
	if (e.x < x.x) r.x = 1;
	if (e.y < x.y) r.y = 1;
	if (e.z < x.z) r.x = 1;
	return r;
}

vec3f abs(vec3f v) {
	vec3f r(abs(v.x), abs(v.y), abs(v.z));
	return r;
}