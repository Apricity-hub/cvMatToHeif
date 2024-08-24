#include <libheif/heif.h>
#include "HeifToMat.h"

cv::Mat HeifToRGBMat(const char* filename)
{
	// 创建HEIF上下文
	heif_context* ctx = heif_context_alloc();
	heif_error error = heif_context_read_from_file(ctx, filename, nullptr);
	if (error.code != heif_error_Ok)
	{
		std::cerr << "读取heif文件失败：" << error.message << std::endl;
		heif_context_free(ctx);
		return cv::Mat();
	}

	// 获取图像句柄
	heif_image_handle* handle;
	error = heif_context_get_primary_image_handle(ctx, &handle);
	if (error.code != heif_error_Ok)
	{
		std::cerr << "获取图像句柄失败：" << error.message << std::endl;
		heif_context_free(ctx);
		return cv::Mat();
	}

	// 解码图像
	heif_image* image;
	error = heif_decode_image(handle, &image, heif_colorspace_RGB, heif_chroma_interleaved_RGB, nullptr);
	if (error.code != heif_error_Ok)
	{
		std::cerr << "解码图像失败：" << error.message << std::endl;
		heif_image_handle_release(handle);
		heif_context_free(ctx);
		return cv::Mat();
	}

	// 获取图像宽度、高度和数据
	int width = heif_image_get_width(image, heif_channel_interleaved);
	int height = heif_image_get_height(image, heif_channel_interleaved);
	int stride;
	const uint8_t* data = heif_image_get_plane_readonly(image, heif_channel_interleaved, &stride);

	// 将图像数据复制到cv::Mat
	cv::Mat mat(height, width, CV_8UC3);
	for (int y = 0; y < height; y++) {
		memcpy(mat.ptr(y), data + y * stride, width * 3);
	}

	cv::cvtColor(mat, mat, cv::COLOR_BGR2RGB);

	//释放资源
	heif_image_release(image);
	heif_image_handle_release(handle);
	heif_context_free(ctx);

	return mat;
}

cv::Mat HeifToGRAYMat(const char* filename)
{
	// 创建HEIF上下文
	heif_context* ctx = heif_context_alloc();
	heif_error error = heif_context_read_from_file(ctx, filename, nullptr);
	if (error.code != heif_error_Ok) {
		std::cerr << "读取HEIF文件失败: " << error.message << std::endl;
		heif_context_free(ctx);
		return cv::Mat();
	}

	// 获取图像句柄
	heif_image_handle* handle;
	error = heif_context_get_primary_image_handle(ctx, &handle);
	if (error.code != heif_error_Ok) {
		std::cerr << "获取图像句柄失败：" << error.message << std::endl;
		heif_context_free(ctx);
		return cv::Mat();
	}

	// 解码图像
	heif_image* image;
	error = heif_decode_image(handle, &image, heif_colorspace_monochrome, heif_chroma_monochrome, nullptr);
	if (error.code != heif_error_Ok) {
		std::cerr << "解码图像失败: " << error.message << std::endl;
		heif_image_handle_release(handle);
		heif_context_free(ctx);
		return cv::Mat();
	}

	// 获取图像宽度、高度和数据
	int width = heif_image_get_width(image, heif_channel_Y);
	int height = heif_image_get_height(image, heif_channel_Y);
	int stride;
	const uint8_t* data = heif_image_get_plane_readonly(image, heif_channel_Y, &stride);

	// 将图像数据复制到 cv::Mat
	cv::Mat mat(height, width, CV_8UC1);
	for (int y = 0; y < height; y++) {
		memcpy(mat.ptr(y), data + y * stride, width);
	}

	cv::imwrite("C:\\Users\\Lenovo\\Desktop\\opencv_decode.png", mat);

	// 释放资源
	heif_image_release(image);
	heif_image_handle_release(handle);
	heif_context_free(ctx);

	return mat;
}