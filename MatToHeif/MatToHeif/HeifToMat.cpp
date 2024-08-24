#include <libheif/heif.h>
#include "HeifToMat.h"

cv::Mat HeifToRGBMat(const char* filename)
{
	// ����HEIF������
	heif_context* ctx = heif_context_alloc();
	heif_error error = heif_context_read_from_file(ctx, filename, nullptr);
	if (error.code != heif_error_Ok)
	{
		std::cerr << "��ȡheif�ļ�ʧ�ܣ�" << error.message << std::endl;
		heif_context_free(ctx);
		return cv::Mat();
	}

	// ��ȡͼ����
	heif_image_handle* handle;
	error = heif_context_get_primary_image_handle(ctx, &handle);
	if (error.code != heif_error_Ok)
	{
		std::cerr << "��ȡͼ����ʧ�ܣ�" << error.message << std::endl;
		heif_context_free(ctx);
		return cv::Mat();
	}

	// ����ͼ��
	heif_image* image;
	error = heif_decode_image(handle, &image, heif_colorspace_RGB, heif_chroma_interleaved_RGB, nullptr);
	if (error.code != heif_error_Ok)
	{
		std::cerr << "����ͼ��ʧ�ܣ�" << error.message << std::endl;
		heif_image_handle_release(handle);
		heif_context_free(ctx);
		return cv::Mat();
	}

	// ��ȡͼ���ȡ��߶Ⱥ�����
	int width = heif_image_get_width(image, heif_channel_interleaved);
	int height = heif_image_get_height(image, heif_channel_interleaved);
	int stride;
	const uint8_t* data = heif_image_get_plane_readonly(image, heif_channel_interleaved, &stride);

	// ��ͼ�����ݸ��Ƶ�cv::Mat
	cv::Mat mat(height, width, CV_8UC3);
	for (int y = 0; y < height; y++) {
		memcpy(mat.ptr(y), data + y * stride, width * 3);
	}

	cv::cvtColor(mat, mat, cv::COLOR_BGR2RGB);

	//�ͷ���Դ
	heif_image_release(image);
	heif_image_handle_release(handle);
	heif_context_free(ctx);

	return mat;
}

cv::Mat HeifToGRAYMat(const char* filename)
{
	// ����HEIF������
	heif_context* ctx = heif_context_alloc();
	heif_error error = heif_context_read_from_file(ctx, filename, nullptr);
	if (error.code != heif_error_Ok) {
		std::cerr << "��ȡHEIF�ļ�ʧ��: " << error.message << std::endl;
		heif_context_free(ctx);
		return cv::Mat();
	}

	// ��ȡͼ����
	heif_image_handle* handle;
	error = heif_context_get_primary_image_handle(ctx, &handle);
	if (error.code != heif_error_Ok) {
		std::cerr << "��ȡͼ����ʧ�ܣ�" << error.message << std::endl;
		heif_context_free(ctx);
		return cv::Mat();
	}

	// ����ͼ��
	heif_image* image;
	error = heif_decode_image(handle, &image, heif_colorspace_monochrome, heif_chroma_monochrome, nullptr);
	if (error.code != heif_error_Ok) {
		std::cerr << "����ͼ��ʧ��: " << error.message << std::endl;
		heif_image_handle_release(handle);
		heif_context_free(ctx);
		return cv::Mat();
	}

	// ��ȡͼ���ȡ��߶Ⱥ�����
	int width = heif_image_get_width(image, heif_channel_Y);
	int height = heif_image_get_height(image, heif_channel_Y);
	int stride;
	const uint8_t* data = heif_image_get_plane_readonly(image, heif_channel_Y, &stride);

	// ��ͼ�����ݸ��Ƶ� cv::Mat
	cv::Mat mat(height, width, CV_8UC1);
	for (int y = 0; y < height; y++) {
		memcpy(mat.ptr(y), data + y * stride, width);
	}

	cv::imwrite("C:\\Users\\Lenovo\\Desktop\\opencv_decode.png", mat);

	// �ͷ���Դ
	heif_image_release(image);
	heif_image_handle_release(handle);
	heif_context_free(ctx);

	return mat;
}