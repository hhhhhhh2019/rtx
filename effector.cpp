#include "bitmap.cpp"
#include "NeuralNetwork.cpp"


bitmap input("render_output.bmp");
bitmap *output = NULL;


int main() {
	network nn(new int[5]{12, 12, 12, 12, 12}, 5);

	for (int y = 0; y < input.height(); y += 2) {
		for (int x = 0; x < input.width(); x += 2) {
			float data[4 * 3] = {input.red(x, y) / 255.,         input.green(x, y) / 255.,         input.blue(x, y) / 255., 
								 input.red(x + 1, y) / 255.,     input.green(x + 1, y) / 255.,     input.blue(x + 1, y) / 255.,
								 input.red(x, y + 1) / 255.,     input.green(x, y + 1) / 255.,     input.blue(x, y + 1) / 255.,
								 input.red(x + 1, y + 1) / 255., input.green(x + 1, y + 1) / 255., input.blue(x + 1, y + 1) / 255.};
			
			nn.one_train(data, data, 0.5);
		}
	}

	output = new bitmap(input.width(), input.height());

	for (int y = 0; y < input.height(); y += 2) {
		for (int x = 0; x < input.width(); x += 2) {
			float data[4 * 3] = {input.red(x, y) / 255.,         input.green(x, y) / 255.,         input.blue(x, y) / 255., 
								 input.red(x + 1, y) / 255.,     input.green(x + 1, y) / 255.,     input.blue(x + 1, y) / 255.,
								 input.red(x, y + 1) / 255.,     input.green(x, y + 1) / 255.,     input.blue(x, y + 1) / 255.,
								 input.red(x + 1, y + 1) / 255., input.green(x + 1, y + 1) / 255., input.blue(x + 1, y + 1) / 255.};
			
			float out[4 * 3];
			nn.run(data, out);

			output->set(x, y, out[0] * 255, out[1] * 255, out[2] * 255);
			output->set(x + 1, y, out[3] * 255, out[4] * 255, out[5] * 255);
			output->set(x, y + 1, out[6] * 255, out[7] * 255, out[8] * 255);
			output->set(x + 1, y + 1, out[9] * 255, out[10] * 255, out[11] * 255);
		}
	}

	output->save("output.bmp");
}