#include <random>
#include <vector>
#include <string.h>


std::random_device rd;
std::default_random_engine eng(rd());
std::uniform_real_distribution<float> distr(-1, 1);


float activate(float x) {
	if (x > 1) return 1. + 0.01 * (x - 1);
	if (x < 0) return 0.01 * x;
	return x;
}

float measure(float x, float y, float e) {
	float a = 1.;
	if (x > 1. || x < 0.) {a = 0.01;}
	return x * y * e * a;
}


// weights
// a  c
// b  d

//   a b
// c w w
// d w w

struct network {
	int laysCount;
	std::vector<int> neuronsCount;
	std::vector<float> neurons;
	std::vector<float> weights;

	network(int nc[], int lc) {
		for (int i = 0; i < lc; i++)
			neuronsCount.insert(neuronsCount.end(), nc[i]);
		laysCount = lc;

		int allNC = nc[0];
		int allWC = 0;

		for (int i = 1; i < lc; i++) {
			allNC += nc[i];
			allWC += nc[i - 1] * nc[i];
		}

		//neurons = new float[allNC * 2];
		//weights = new float[allWC];

		for (int i = 0 ; i < allNC * 2; i++) neurons.insert(neurons.end(), 0);

		for (int i = 0; i < allWC; i++) weights.insert(weights.end(), distr(eng));
	}

	// lay1 - output, lay2 - input
	void forwards(int lay1, int lay2) {
		int wfst = 0;
		for (int i = 1; i < lay2; i++) wfst += neuronsCount[i] * neuronsCount[i - 1];

		int fst1 = 0;
		for (int i = 0; i < lay1; i++) fst1 += neuronsCount[i] * 2;

		int fst2 = 0;
		for (int i = 0; i < lay2; i++) fst2 += neuronsCount[i] * 2;

		for (int i = 0; i < neuronsCount[lay1]; i++) {
			neurons[fst1 + i * 2] = 0;
			for (int j = 0; j < neuronsCount[lay2]; j++) {
				neurons[fst1 + i * 2] += neurons[fst2 + j * 2] * weights[wfst + j * neuronsCount[lay1] + i];
			}
			neurons[fst1 + i * 2] = activate(neurons[fst1 + i * 2]);
		}
	}


	void run(float *data, float *out) {
		for (int i = 0; i < neuronsCount[0]; i++) {
			neurons[i * 2] = data[i];
		}

		for (int i = 1; i < laysCount; i++) {
			forwards(i, i - 1);
		}


		int oFst = 0;
		for (int i = 0; i < laysCount - 1; i++)
			oFst += neuronsCount[i] * 2;

		for (int i = 0; i < neuronsCount[laysCount - 1]; i++) {
			out[i] = neurons[oFst + i * 2];
		}
	}

	void find_out_error(float *need) {
		int oFst = 0;
		for (int i = 0; i < laysCount - 1; i++)
			oFst += neuronsCount[i] * 2;

		for (int i = 0; i < neuronsCount[laysCount - 1]; i++) {
			neurons[oFst + i * 2 + 1] = need[i] - neurons[oFst + i * 2];
		}
	}

	void one_train(float *inp, float *need, float k) {
		float *out = need;
		run(inp, out);

		find_out_error(need);


	}


	void train(float *ld, int ds, float k) {
		for (int i = 0; i < sizeof(ld) * sizeof(ld[0]) / ds; i+=2) {
			float inp[ds];
			float out[ds];
			memcpy(inp, ld + i * ds, ds * sizeof(float));
			memcpy(out, ld + i * ds + ds, ds * sizeof(float));
			one_train(inp, out, k);
		}
	}
};