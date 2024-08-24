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

	// 创建HEIF图像
	err = heif_image_create(width, height, heif_colorspace_RGB, heif_chroma_interleaved_RGB, &image);
	if (err.code != heif_error_Ok)
	{
		std::cerr << "Error adding plane to HEIF image：" << err.message << std::endl;
		heif_image_release(image);
		heif_context_free(ctx);
		return false;
	}

	// 添加一个用于交错 RGB 通道的图像平面
	err = heif_image_add_plane(image, heif_channel_interleaved, width, height, 8);
	if (err.code != heif_error_Ok) {
		std::cerr << "Error adding plane to HEIF image: " << err.message << std::endl;
		heif_image_release(image);
		heif_context_free(ctx);
		return false;
	}


	// 获取指向HEIF图像数据的指针
	int stride;
	uint8_t* p = heif_image_get_plane(image, heif_channel_interleaved, &stride);

	// 将 cv::Mat 数据复制到 HEIF 图像中
	for (int y = 0; y < height; y++) {
		memcpy(p + y * stride, mat.ptr(y), width * 3);  // 每行的像素数据都是 width * 3 个字节
	}


	//for (int y = 0; y < height; y++) {
	//	// 使用 size_t 来安全地计算偏移量  
	//	size_t rowOffset = static_cast<size_t>(y) * stride;
	//	// 将偏移量转换为 char* 类型的指针偏移  
	//	char* rowPtr = reinterpret_cast<char*>(p) + rowOffset;
	//	// 使用 memcpy 复制数据  
	//	memcpy(rowPtr, mat.ptr<uchar>(y), width * 3);  // 注意：这里假设 mat.ptr 返回 uchar*  
	//	// 注意：如果 mat.ptr 实际上返回的是其他类型的指针（如 RGB 像素的 struct 或类），  
	//	// 你可能需要调整 memcpy 的第三个参数来匹配实际的字节大小。  
	//}

	//// 创建并初始化heif_encoding_options结构体
	//struct heif_encoding_options encoding_options = { 0 };
	//encoding_options.version = 0;
	//encoding_options.save_alpha_channel = false;
	//encoding_options.image_orientation = heif_orientation_normal; // 例如：设置图像方向为正常
	
	// 创建HEIF编码器
	struct heif_encoder* encoder;

	err = heif_context_get_encoder_for_format(ctx, heif_compression_HEVC, &encoder);
	if (err.code != heif_error_Ok)
	{
		std::cerr << "Error getting HEIF encoder: " << err.message << std::endl;
		heif_image_release(image);
		heif_context_free(ctx);
		return false;
	}

	heif_encoder_set_lossy_quality(encoder, 80);  //0-100 压缩比例
	heif_encoder_set_lossless(encoder, true);     // True / False
	heif_encoder_set_logging_level(encoder, 4);   // log信息详细程度0-4

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
	// 确保输入的cv::Mat 是8位深度，单通道的灰度图
	if (mat.empty() || mat.type() != CV_8UC1) {
		std::cerr << "Invalid Mat format. Expected 8UC1." << std::endl;
		return false;
	}

	struct heif_image* image;
	struct heif_context* ctx = heif_context_alloc();
	struct heif_error err;

	int width = mat.cols;
	int height = mat.rows;

	// 创建HEIF图像
	err = heif_image_create(width, height, heif_colorspace_monochrome, heif_chroma_monochrome, &image);
	if (err.code != heif_error_Ok) {
		std::cerr << "Error creating HEIF image: " << err.message << std::endl;
		heif_context_free(ctx);
		return false;
	}

	// 添加一个用于灰度通道的图像平面
	err = heif_image_add_plane(image, heif_channel_Y, width, height, 8);
	if (err.code != heif_error_Ok) {
		std::cerr << "Error adding plane to HEIF image: " << err.message << std::endl;
		heif_image_release(image);
		heif_context_free(ctx);
		return false;
	}

	// 获取指向HEIF图像灰度数据的指针
	int stride;
	uint8_t* p = heif_image_get_plane(image, heif_channel_Y, &stride);

	// 将cv::Mat 的灰度数据复制到HEIF图像中
	for (int y = 0; y < height; y++) {
		memcpy(p + y * stride, mat.ptr(y), width);  // 每行的像素数据是 width 个字节
	}

	// 创建HEIF编码器
	struct heif_encoder* encoder;
	err = heif_context_get_encoder_for_format(ctx, heif_compression_HEVC, &encoder);
	if (err.code != heif_error_Ok) {
		std::cerr << "Error getting HEIF encoder: " << err.message << std::endl;
		heif_image_release(image);
		heif_context_free(ctx);
		return false;
	}

	heif_encoder_set_lossy_quality(encoder, 80);  //0-100 压缩比例
	heif_encoder_set_lossless(encoder, true);     // True / False
	heif_encoder_set_logging_level(encoder, 4);   // log信息详细程度0-4

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