#include <libheif/heif.h>
#include "MatToHeif.h"

bool RGBMatToHeif(cv::Mat& mat, const char* filename)
{
	if (mat.empty() || mat.type() != CV_8UC3)
	{
		std::cerr << "Invalid Mat format. Expected 8UC3." << std::endl;
		return false;
	}

	struct heif_image* image;
	struct heif_context* ctx = heif_context_alloc();
	struct heif_error err;

	int width = mat.cols;
	int height = mat.rows;

	// ����HEIFͼ��
	err = heif_image_create(width, height, heif_colorspace_RGB, heif_chroma_interleaved_RGB, &image);
	if (err.code != heif_error_Ok)
	{
		std::cerr << "Error adding plane to HEIF image��" << err.message << std::endl;
		heif_image_release(image);
		heif_context_free(ctx);
		return false;
	}

	// ���һ�����ڽ��� RGB ͨ����ͼ��ƽ��
	err = heif_image_add_plane(image, heif_channel_interleaved, width, height, 8);
	if (err.code != heif_error_Ok) {
		std::cerr << "Error adding plane to HEIF image: " << err.message << std::endl;
		heif_image_release(image);
		heif_context_free(ctx);
		return false;
	}


	// ��ȡָ��HEIFͼ�����ݵ�ָ��
	int stride;
	uint8_t* p = heif_image_get_plane(image, heif_channel_interleaved, &stride);

	// �� cv::Mat ���ݸ��Ƶ� HEIF ͼ����
	for (int y = 0; y < height; y++) {
		memcpy(p + y * stride, mat.ptr(y), width * 3);  // ÿ�е��������ݶ��� width * 3 ���ֽ�
	}


	//for (int y = 0; y < height; y++) {
	//	// ʹ�� size_t ����ȫ�ؼ���ƫ����  
	//	size_t rowOffset = static_cast<size_t>(y) * stride;
	//	// ��ƫ����ת��Ϊ char* ���͵�ָ��ƫ��  
	//	char* rowPtr = reinterpret_cast<char*>(p) + rowOffset;
	//	// ʹ�� memcpy ��������  
	//	memcpy(rowPtr, mat.ptr<uchar>(y), width * 3);  // ע�⣺������� mat.ptr ���� uchar*  
	//	// ע�⣺��� mat.ptr ʵ���Ϸ��ص����������͵�ָ�루�� RGB ���ص� struct ���ࣩ��  
	//	// �������Ҫ���� memcpy �ĵ�����������ƥ��ʵ�ʵ��ֽڴ�С��  
	//}

	//// ��������ʼ��heif_encoding_options�ṹ��
	//struct heif_encoding_options encoding_options = { 0 };
	//encoding_options.version = 0;
	//encoding_options.save_alpha_channel = false;
	//encoding_options.image_orientation = heif_orientation_normal; // ���磺����ͼ����Ϊ����
	
	// ����HEIF������
	struct heif_encoder* encoder;

	err = heif_context_get_encoder_for_format(ctx, heif_compression_HEVC, &encoder);
	if (err.code != heif_error_Ok)
	{
		std::cerr << "Error getting HEIF encoder: " << err.message << std::endl;
		heif_image_release(image);
		heif_context_free(ctx);
		return false;
	}

	heif_encoder_set_lossy_quality(encoder, 80);  //0-100 ѹ������
	heif_encoder_set_lossless(encoder, true);     // True / False
	heif_encoder_set_logging_level(encoder, 4);   // log��Ϣ��ϸ�̶�0-4

	err = heif_context_encode_image(ctx, image, encoder, nullptr, nullptr);
	if (err.code != heif_error_Ok) {
		std::cerr << "Error encoding HEIF image: " << err.message << std::endl;
		heif_encoder_release(encoder);
		heif_image_release(image);
		heif_context_free(ctx);
		return false;
	}

	err = heif_context_write_to_file(ctx, filename);

	if (err.code != heif_error_Ok) {
		std::cerr << "Error writing HEIF file: " << err.message << std::endl;
		heif_encoder_release(encoder);
		heif_image_release(image);
		heif_context_free(ctx);
		return false;
	}

	heif_encoder_release(encoder);
	heif_image_release(image);
	heif_context_free(ctx);

	return true;
}

bool GRAYMatToHeif(cv::Mat& mat, const char* filename)
{
	// ȷ�������cv::Mat ��8λ��ȣ���ͨ���ĻҶ�ͼ
	if (mat.empty() || mat.type() != CV_8UC1) {
		std::cerr << "Invalid Mat format. Expected 8UC1." << std::endl;
		return false;
	}

	struct heif_image* image;
	struct heif_context* ctx = heif_context_alloc();
	struct heif_error err;

	int width = mat.cols;
	int height = mat.rows;

	// ����HEIFͼ��
	err = heif_image_create(width, height, heif_colorspace_monochrome, heif_chroma_monochrome, &image);
	if (err.code != heif_error_Ok) {
		std::cerr << "Error creating HEIF image: " << err.message << std::endl;
		heif_context_free(ctx);
		return false;
	}

	// ���һ�����ڻҶ�ͨ����ͼ��ƽ��
	err = heif_image_add_plane(image, heif_channel_Y, width, height, 8);
	if (err.code != heif_error_Ok) {
		std::cerr << "Error adding plane to HEIF image: " << err.message << std::endl;
		heif_image_release(image);
		heif_context_free(ctx);
		return false;
	}

	// ��ȡָ��HEIFͼ��Ҷ����ݵ�ָ��
	int stride;
	uint8_t* p = heif_image_get_plane(image, heif_channel_Y, &stride);

	// ��cv::Mat �ĻҶ����ݸ��Ƶ�HEIFͼ����
	for (int y = 0; y < height; y++) {
		memcpy(p + y * stride, mat.ptr(y), width);  // ÿ�е����������� width ���ֽ�
	}

	// ����HEIF������
	struct heif_encoder* encoder;
	err = heif_context_get_encoder_for_format(ctx, heif_compression_HEVC, &encoder);
	if (err.code != heif_error_Ok) {
		std::cerr << "Error getting HEIF encoder: " << err.message << std::endl;
		heif_image_release(image);
		heif_context_free(ctx);
		return false;
	}

	heif_encoder_set_lossy_quality(encoder, 80);  //0-100 ѹ������
	heif_encoder_set_lossless(encoder, true);     // True / False
	heif_encoder_set_logging_level(encoder, 4);   // log��Ϣ��ϸ�̶�0-4

	err = heif_context_encode_image(ctx, image, encoder, nullptr, nullptr);
	if (err.code != heif_error_Ok) {
		std::cerr << "Error encoding HEIF image: " << err.message << std::endl;
		heif_encoder_release(encoder);
		heif_image_release(image);
		heif_context_free(ctx);
		return false;
	}

	err = heif_context_write_to_file(ctx, filename);

	if (err.code != heif_error_Ok) {
		std::cerr << "Error writing HEIF file: " << err.message << std::endl;
		heif_encoder_release(encoder);
		heif_image_release(image);
		heif_context_free(ctx);
		return false;
	}

	heif_encoder_release(encoder);
	heif_image_release(image);
	heif_context_free(ctx);

	return true;

}