#include "bitmap.cpp"
#include "vector.cpp"
#include <random>


#define width 720
#define height 480
#define depth 1000

bitmap output(width, height);

vec3f light = normalize(vec3f(0,1,1));


std::random_device rd;
std::default_random_engine eng(rd());
std::uniform_real_distribution<float> distr(0, 1);


vec2f sphere(vec3f ro, vec3f rd, float ra) {
	float b = dot(ro, rd);
	float c = dot(ro, ro) - ra * ra;
	float h = b * b - c;
	if(h < 0.0) return vec2f(-1.0);
	h = sqrt(h);
	return vec2f(-b - h, -b + h);
}

float plane(vec3f ro, vec3f rd, vec3f p, float d) {
	return -(dot(ro, p) + d) / dot(rd, p);
}

vec2f box(vec3f ro, vec3f rd, vec3f rad, vec3f& oN)  {
	vec3f m = vec3f(1.0) / rd;
	vec3f n = m * ro;
	vec3f k = rad * abs(m);
	vec3f t1 = vec3f(0)-n - k;
	vec3f t2 = vec3f(0)-n + k;
	float tN = fmax(fmax(t1.x, t1.y), t1.z);
	float tF = fmin(fmin(t2.x, t2.y), t2.z);
	if(tN > tF || tF < 0.0) return vec2f(-1.0);
	oN = (vec3f(0)-sign(rd)) * step(vec3f(t1.y, t1.z, t1.x), t1) * step(vec3f(t1.z, t1.x, t1.y), t1);
	return vec2f(tN, tF);
}

vec3f getSky(vec3f rd) {
	vec3f col = vec3f(.4, .8, 1);
	return col;
	vec3f sun = vec3f(0.95, 0.9, 1.0);
	sun = sun * fmax(0.0, pow(dot(rd, light), 256.0));
	col = col * fmax(0.0, dot(light, vec3f(0.0, 0.0, -1.0)));
	return sun + col * 0.01;
}

vec3f reflect(vec3f r, vec3f n) {
	return r - n * 2. * dot(r, n);
}

vec3f randomOnSphere() {
	vec3f rand = vec3f(distr(eng), distr(eng), distr(eng));
	float theta = rand.x * 6.28318531;
	float v = rand.y;
	float phi = acos(2.0 * v - 1.0);
	float r = pow(rand.z, 0.333333333333);
	float x = r * sin(phi) * cos(theta);
	float y = r * sin(phi) * sin(theta);
	float z = r * cos(phi);
	return normalize(vec3f(x, y, z));
}

vec4f trayce(vec3f& ro, vec3f& rd) {
	vec2f minIt(depth);
	vec4f col;
	vec2f it;
	vec3f n;
	
	
	it = sphere(ro - vec3f(-1,0,3), rd, 1.);
	if (it.x > 0. && it.x < minIt.x) {
		minIt = it;
		col = vec4f(1,.2,.2, 0);
		n = normalize(ro + rd * it.x - vec3f(-1,0,3));
	}
	
	it = sphere(ro - vec3f(1,0,4), rd, 1.);
	if (it.x > 0. && it.x < minIt.x) {
		minIt = it;
		col = vec4f(.2,1,.2, 0.9);
		n = normalize(ro + rd * it.x - vec3f(1,0,4));
	}
	
	vec3f bn;
	it = box(ro - vec3f(0,-2,4), rd, vec3f(1,.1,1), bn);
	if (it.x > 0. && it.x < minIt.x) {
		minIt = it;
		col = vec4f(1,1,1, -2);
		n = bn;
	}
	
	
	// down
	it = vec2f(plane(ro, rd, vec3f(0,1,0),-1));
	if (it.x > 0. && it.x < minIt.x) {
		minIt = it;
		col = vec4f(.5,.5,.5, 0);
		n = vec3f(0,1,0);
	}
	
	// up
	it = vec2f(plane(ro, rd, vec3f(0,-1,0),-2));
	if (it.x > 0. && it.x < minIt.x) {
		minIt = it;
		col = vec4f(.5,.5,.5, 0);
		n = vec3f(0,-1,0);
	}
	
	// left
	it = vec2f(plane(ro, rd, vec3f(1,0,0),-3));
	if (it.x > 0. && it.x < minIt.x) {
		minIt = it;
		col = vec4f(.1,.6,1, 0);
		n = vec3f(1,0,0);
	}
	
	// right
	it = vec2f(plane(ro, rd, vec3f(-1,0,0),-3));
	if (it.x > 0. && it.x < minIt.x) {
		minIt = it;
		col = vec4f(1,.7,.1, 0);
		n = vec3f(-1,0,0);
	}
	
	
	// front
	it = vec2f(plane(ro, rd, vec3f(0,0,1),-5));
	if (it.x > 0. && it.x < minIt.x) {
		minIt = it;
		col = vec4f(.5,.5,.5, 0);
		n = vec3f(0,0,1);
	}
	
	// back
	it = vec2f(plane(ro, rd, vec3f(0,0,-1),-3));
	if (it.x > 0. && it.x < minIt.x) {
		minIt = it;
		col = vec4f(.5,.5,.5, 0);
		n = vec3f(0,0,-1);
	}
	
	
	
	if (col.w == -2.) return col;
	
	if (minIt.x == depth) {
		vec3f c = getSky(rd);
		return vec4f(c.x, c.y, c.z, -2);
	}
	
	n = n * (-sign(dot(rd, n)));
	
	vec3f reflected = reflect(rd, n);
	vec3f r = randomOnSphere();
	//vec3f diffuse = normalize(vec3f(distr(eng), distr(eng), distr(eng)));
	vec3f diffuse = normalize(r * dot(r, n));
	
	ro = ro + rd * (minIt.x - .001);
	rd = mix(reflected, diffuse, col.w);
	
	return col;
}

vec3f render(vec3f ro, vec3f rd) {
	vec3f col(1);
	
	for (int i = 0; i < 4; i++) {
		vec4f nc = trayce(ro, rd);
		col = col * vec3f(nc.x, nc.y, nc.z);
		if (nc.w == -2.) return col;
	}
	
	return vec3f(0);
}

int main() {
	for (int y = 0; y < height; y++) {
		#pragma omp parrallel
		for (int x = 0; x < width; x++) {
			vec3f ro(0,-1,-1);
			//vec3f rd = normalize(vec3f((float)x/width*2-1, (float)y/height*2-1, 1));
			vec2f uv = vec2f(x, y) / vec2f(width, height) * 2. - vec2f(1);
			uv.x *= (float)width / (float)height;
			vec3f rd = normalize(vec3f(uv.x, uv.y, 1));
			
			float a = 3.14159 * -0.1;
			
			rd = rd * mat3(
				1,0,0,
				0,cos(a),-sin(a),
				0,sin(a),cos(a)
			);
			
			vec3f col(0);
			
			int samples = 512;
			
			for (int i = 0; i < samples; i++) {
				col = col + render(ro, rd);
			}
			
			col = col / (float)samples;
			
			output.set(x,y,sqrt(col.x)*255,sqrt(col.y)*255,sqrt(col.z)*255);
		}
		
		printf("Осталось %i строк\n", height - y - 1);
	}
	
	output.save("render_output.bmp");
	
	return 0;
}