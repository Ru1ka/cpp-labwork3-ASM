#include "IO_manager.h"

struct ParsedLine {
    uint16_t x = 0;
    uint16_t y = 0;
    uint64_t value = 0;
};

ParsedLine parseLine(char* line) {
    ParsedLine result;

    result.x = atoi(line);

    line = strchr(line, '\t') + 1;
    result.y = atoi(line);

    line = strchr(line, '\t') + 1;
    result.value = atoi(line);

    return result;
}

const uint8_t MAX_LINE_SIZE = 40;
const int BORDER_SIZE = 1;

TSVData readTSV(char* input_file) {
    FILE* file = std::fopen(input_file, "r");
    if (file == nullptr) {
        throw std::runtime_error("ERROR: Failed to open input file.");
    }
    char line[MAX_LINE_SIZE];
    uint16_t min_x = std::numeric_limits<uint16_t>::max();
    uint16_t min_y = std::numeric_limits<uint16_t>::max();
    uint16_t max_x = std::numeric_limits<uint16_t>::min();
    uint16_t max_y = std::numeric_limits<uint16_t>::min();
    while (std::fgets(line, sizeof(line), file)) {
        ParsedLine data = parseLine(line);
        if (data.x > max_x) max_x = data.x;
        if (data.x < min_x) min_x = data.x;
        if (data.y > max_y) max_y = data.y;
        if (data.y < min_y) min_y = data.y;
    }

    uint16_t size_y = max_y - min_y + 1 + BORDER_SIZE * 2;
    uint16_t size_x = max_x - min_x + 1 + BORDER_SIZE * 2;

    TSVData result;
    result.matrix = new Matrix(size_y, size_x);

    std::fclose(file);
    file = std::fopen(input_file, "r");
    while (std::fgets(line, sizeof(line), file)) {
        ParsedLine line_data = parseLine(line);
        (*result.matrix)[BORDER_SIZE + line_data.y - min_y][BORDER_SIZE + line_data.x - min_x] = line_data.value;
        if (line_data.value >= 4) {
            result.iter_data.push(Coords(BORDER_SIZE + line_data.y - min_y, BORDER_SIZE + line_data.x - min_x));
        }
    }

    return result;
}

const char BMP_SIGNATURE[] = "BM";       // always "BM" (2 bytes "B" and "M" indexes in ASCII)
const uint32_t HEADER_SIZE = 14;         // always 14
const uint32_t INFO_HEADER_SIZE = 40;    // always 40
const uint32_t RESERVED_BYTES = 0x0000;  // always 0x0000 (0), for bytes actually reserved for smth
const uint16_t BITS_PER_PIXEL = 4;       // according to README.md
const uint16_t PLANES = 1;               // always 1
const uint32_t COMPRESSION = 0;          // because img isn't compressed
const uint32_t PPM = 2835;               // default, 72DPI
const uint32_t COLORS_USED = 5;          // according to README.md
const uint32_t IMPORTANT_COLORS = 0;     // all сolors important
const uint32_t COLOR_PALETTE[5] = {
    0xFFFFFF,  // White
    0x6AA84F,  // Green
    0x8E7CC3,  // Purple
    0xF1C232,  // Yellow
    0x000000   // Black
};

void saveBMP(char* path, Matrix* data) {
    uint16_t min_x = std::numeric_limits<uint16_t>::max();
    uint16_t min_y = std::numeric_limits<uint16_t>::max();
    uint16_t max_x = std::numeric_limits<uint16_t>::min();
    uint16_t max_y = std::numeric_limits<uint16_t>::min();
    for (int i = 0; i < data->size_y; ++i) {
        for (int j = 0; j < data->size_x; ++j) {
            if ((*data)[i][j] != 0) {
                if (j > max_x) max_x = j;
                if (j < min_x) min_x = j;
                if (i > max_y) max_y = i;
                if (i < min_y) min_y = i;
            }
        }
    }
    FILE* output_file = std::fopen(path, "wb+");
    if (output_file == nullptr) {
        throw std::runtime_error("ERROR: Failed open output file.");
    }
    uint32_t size_x = max_x - min_x + 1;
    uint32_t size_y = max_y - min_y + 1;
    uint32_t row_size = (size_x + 1) / 2;
    uint32_t padding = (4 - (row_size % 4)) % 4;
    uint32_t image_size = (row_size + padding) * size_y;
    uint32_t file_size = HEADER_SIZE + INFO_HEADER_SIZE + COLORS_USED * sizeof(uint32_t) + image_size;
    uint32_t data_offset = HEADER_SIZE + INFO_HEADER_SIZE + COLORS_USED * sizeof(uint32_t);
    // header
    std::fwrite(&BMP_SIGNATURE, sizeof(char), sizeof(BMP_SIGNATURE) - 1, output_file);
    std::fwrite(&file_size, sizeof(uint32_t), 1, output_file);
    std::fwrite(&RESERVED_BYTES, sizeof(uint32_t), 1, output_file);
    std::fwrite(&data_offset, sizeof(uint32_t), 1, output_file);
    // Info header
    std::fwrite(&INFO_HEADER_SIZE, sizeof(uint32_t), 1, output_file);
    std::fwrite(&size_x, sizeof(uint32_t), 1, output_file);
    std::fwrite(&size_y, sizeof(uint32_t), 1, output_file);
    std::fwrite(&PLANES, sizeof(uint16_t), 1, output_file);
    std::fwrite(&BITS_PER_PIXEL, sizeof(uint16_t), 1, output_file);
    std::fwrite(&COMPRESSION, sizeof(uint32_t), 1, output_file);
    std::fwrite(&image_size, sizeof(uint32_t), 1, output_file);
    std::fwrite(&PPM, sizeof(uint32_t), 1, output_file);
    std::fwrite(&PPM, sizeof(uint32_t), 1, output_file);
    std::fwrite(&COLORS_USED, sizeof(uint32_t), 1, output_file);
    std::fwrite(&IMPORTANT_COLORS, sizeof(uint32_t), 1, output_file);

    // Color palette
    std::fwrite(COLOR_PALETTE, sizeof(uint32_t), COLORS_USED, output_file);

    for (int y = size_y - 1; y >= 0; --y) {
        for (uint32_t x = 0; x < size_x; x += 2) {  // 2 pixels in byte
            uint8_t byte = 0;
            uint8_t pixel1 = (*data)[min_y + y][min_x + x] <= 3 ? (*data)[min_y + y][min_x + x] : 4;
            uint8_t pixel2 = (*data)[min_y + y][min_x + x + 1] <= 3 ? (*data)[min_y + y][min_x + x + 1] : 4;
            byte = (pixel1 & 0x0F) << 4;

            if (x + 1 < size_x) {
                byte |= (pixel2 & 0x0F);
            }

            std::fwrite(&byte, sizeof(uint8_t), 1, output_file);
        }

        uint8_t padding_bytes[3] = {0, 0, 0};
        std::fwrite(padding_bytes, 1, padding, output_file);
    }
    std::fclose(output_file);
    std::cout << "Saved image in: " << path << "\t:---------:\tImage size: " << size_x << "x" << size_y;
}

TSVData readBMP(char* input_path) {
    FILE* file = std::fopen(input_path, "rb");
    if (file == nullptr) {
        throw std::runtime_error("ERROR: Failed to open input file.");
    }
    char header[HEADER_SIZE];
    std::fread(header, sizeof(char), HEADER_SIZE, file);

    uint32_t dibHeaderSize;
    std::fread(&dibHeaderSize, sizeof(uint32_t), 1, file);

    uint32_t width, height;
    std::fread(&width, sizeof(uint32_t), 1, file);
    std::fread(&height, sizeof(uint32_t), 1, file);

    uint16_t planes, bitCount;
    std::fread(&planes, sizeof(uint16_t), 1, file);
    std::fread(&bitCount, sizeof(uint16_t), 1, file);

    std::fseek(file, dibHeaderSize - 16, SEEK_CUR);

    TSVData result;
    result.matrix = new Matrix(height + (BORDER_SIZE * 2), width + (BORDER_SIZE * 2));

    uint32_t rowPadding = (4 - (width * 3) % 4) % 4;
    for (int y = height - 1; y >= 0; --y) { 
        for (uint32_t x = 0; x < width; ++x) {
            uint8_t blue, green, red;
            std::fread(&blue, sizeof(uint8_t), 1, file);
            std::fread(&green, sizeof(uint8_t), 1, file);
            std::fread(&red, sizeof(uint8_t), 1, file);

            uint16_t value = (red + green + blue);
            (*result.matrix)[BORDER_SIZE + y][BORDER_SIZE + x] = value;
            if (value >= 4) {
                result.iter_data.push(Coords(BORDER_SIZE + y, BORDER_SIZE + x));
            }
        }
        std::fseek(file, rowPadding, SEEK_CUR);
    }

    std::fclose(file);
    return result;
}
