#include "pch.h"
#include<opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

//Pega maior e menor valor de intesidade de pixel de uma imagem
int maior(Mat img);
int menor(Mat img);

//Equalização do histograma
vector<int> histograma(Mat img, vector<int> hist);
vector<int> histogramaAcumulado(Mat img, vector<int> hist);
void equalizacaoDoHistograma(Mat img);

//Histograma de Stretching
void histogramaDeStretching(Mat img);

//Mostra imagem antes de aplicar o algoritmo
void mostraImagemAntes(Mat img);

int main(int argc, const char * argv[]) {

	Mat imgA, imgFinal01;
	int k = 0;

	//Define caminho da imagen
	String imageNameA("data/menina.jpg");

	// Ler imagem
	imgA = imread(imageNameA, IMREAD_GRAYSCALE);


	int option = 0;
	while (option != 3) {
		cout << "ESCOLHA\n\n";
		cout << " 1. Equalizacao de histograma \n 2. histograma stretching \n 3. Sair \n\n Resposta: ";

		cin >> option;
		//option = 1;

		switch (option) {
		case 1:
			equalizacaoDoHistograma(imgA);
			break;
		case 2:
			histogramaDeStretching(imgA);
			break;
		case 3:
			cout << "Programa encerrado";
			break;
		default:
			cout << "Valor invalido";
			break;
		}



		waitKey(0);
	}

	return 0;
}

vector<int> histograma(Mat img, vector<int> hist) {

	int v;

	for (int row = 0; row < img.rows; row++) {
		for (int col = 0; col < img.cols; col++) {
			v = (int)img.at<uchar>(row, col); //Coloca o valor do pixel atual na variável v
			++hist[v];
		}
	}

	return hist;
}

vector<int> histogramaAcumulado(Mat img, vector<int> hist) {

	histograma(img, hist);

	vector<int> histAcumulado(256, 0);

	int sum = 0;
	int valor;
	for (int i = 0; i < hist.size(); ++i) {
		valor = hist[i];
		sum = sum + hist[i];

		histAcumulado[i] = sum;
	}

	return histAcumulado;
}

void equalizacaoDoHistograma(Mat img) {
	vector<int> hist(256, 0);
	vector<int> histogramaNormal(256, 0);
	vector<int> histAcumulado(256, 0);
	vector<int> novoHistograma(256, 0);

	Mat imgEqualizada;
	int valor;

	img.copyTo(imgEqualizada);

	histogramaNormal = histograma(img, hist);

	histAcumulado = histogramaAcumulado(img, histogramaNormal);


	int multRowsCols = img.cols * img.rows;
	for (int greylevel = 0; greylevel < 256; greylevel++) {
		novoHistograma[greylevel] = (histAcumulado[greylevel] * 255) / multRowsCols;
	}

	for (int row = 0; row < img.rows; row++) {
		for (int col = 0; col < img.cols; col++) {
			valor = (int)imgEqualizada.at<uchar>(row, col);
			imgEqualizada.at<uchar>(row, col) = novoHistograma[valor];
		}
	}

	namedWindow("equalizacaoDoHistograma.jpg", WINDOW_AUTOSIZE);
	imshow("equalizacaoDoHistograma.jpg", imgEqualizada);
	imwrite("resultado/equalizacaoDoHistograma.jpg", imgEqualizada); //Salva a imagem

	//Mostra imagem antes de aplicar o algoritmo
	mostraImagemAntes(img);
}

//Pega maior valor de uma matriz
int maior(Mat img) {

	int maior = img.at<uchar>(0, 0);
	int valorIntensidade;


	for (int row = 0; row < img.rows; row++) {
		for (int col = 0; col < img.cols; col++) {
			valorIntensidade = (int)img.at<uchar>(row, col);
			if (valorIntensidade > maior) {
				maior = valorIntensidade;
			}
		}
	}
	return maior;

}

//Pega menor valor de uma matriz
int menor(Mat img) {

	int menor = img.at<uchar>(0, 0);
	int valorIntensidade;


	for (int row = 0; row < img.rows; row++) {
		for (int col = 0; col < img.cols; col++) {
			valorIntensidade = (int)img.at<uchar>(row, col);
			if (valorIntensidade < menor) {
				menor = valorIntensidade;
			}
		}
	}
	return menor;

}

void histogramaDeStretching(Mat img) {
	Mat imgResultado;
	img.copyTo(imgResultado);

	int z, valorPixel, contraste;

	int maiorIntensidadeOriginal = maior(img);
	int menorIntensidadeOriginal = menor(img);
	float pixelSubMenor;
	float contrasteEpixelSub;

	maiorIntensidadeOriginal = maior(img);
	menorIntensidadeOriginal = menor(img);

	contraste = maiorIntensidadeOriginal - menorIntensidadeOriginal;

	for (int row = 0; row < img.rows; row++) {
		for (int col = 0; col < img.cols; col++) {
			valorPixel = (int)img.at<uchar>(row, col);

			pixelSubMenor = valorPixel - menorIntensidadeOriginal;
			contrasteEpixelSub = pixelSubMenor / contraste;

			z = 255 * contrasteEpixelSub;

			imgResultado.at<uchar>(row, col) = z;

		}
	}


	namedWindow("HistStretching.jpg", WINDOW_AUTOSIZE);
	imshow("HistStretching.jpg", imgResultado);
	imwrite("resultado/HistStretching.jpg", imgResultado); //Salva a imagem

	//Mostra imagem antes de aplicar o algoritmo
	mostraImagemAntes(img);
}

//Mostra imagem antes de aplicar o algoritmo
void mostraImagemAntes(Mat img) {
	namedWindow("normal.jpg", WINDOW_AUTOSIZE);
	imshow("normal.jpg", img);
}