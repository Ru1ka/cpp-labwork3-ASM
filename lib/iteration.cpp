#include "iteration.h"

void shiftIndexInRingBuffer(RingBuffer* data, const uint16_t shift_x, const uint16_t shift_y) {
    if (shift_x == 0 && shift_y == 0) return;
    if (data->head <= data->tail) {
        for (uint32_t i = data->head; i < data->tail; i++) {
            data->buffer[i].y += shift_y;
            data->buffer[i].x += shift_x;
        }
    } else {
        for (uint32_t i = data->head; i < data->size; i++) {
            data->buffer[i].y += shift_y;
            data->buffer[i].x += shift_x;
        }
        for (uint32_t i = 0; i < data->tail; i++) {
            data->buffer[i].y += shift_y;
            data->buffer[i].x += shift_x;
        }
    }
}

void iterateLoop(Matrix* data, RingBuffer* ring_buffer, Args CLI_data) {
    uint64_t iteration = 1;
    uint64_t iterations_start = iteration;
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    while ((!ring_buffer->isEmpty() && CLI_data.max_iter == 0) || iteration <= CLI_data.max_iter) {
        bool increase_size_x_plus = false;
        bool increase_size_x_minus = false;
        bool increase_size_y_plus = false;
        bool increase_size_y_minus = false;

        // Iterate black pixels
        uint64_t ring_buffer_size = ring_buffer->len();
        for (int i = 0; i < ring_buffer_size; ++i) {
            Coords cur_coords = (*ring_buffer).pop();
            (*data)[cur_coords.y][cur_coords.x] -= 4;
            // Add current
            if ((*data)[cur_coords.y][cur_coords.x] >= 4) {
                (*ring_buffer).push(cur_coords);
            }
            // Add neighbours
            if (++(*data)[cur_coords.y + 1][cur_coords.x] == 4) {
                (*ring_buffer).push(Coords(cur_coords.y + 1, cur_coords.x));
                if (cur_coords.y + 2 == data->size_y) {
                    increase_size_y_plus = true;
                }
            }
            if (++(*data)[cur_coords.y - 1][cur_coords.x] == 4) {
                (*ring_buffer).push(Coords(cur_coords.y - 1, cur_coords.x));
                if (cur_coords.y - 1 == 0) {
                    increase_size_y_minus = true;
                }
            }
            if (++(*data)[cur_coords.y][cur_coords.x + 1] == 4) {
                (*ring_buffer).push(Coords(cur_coords.y, cur_coords.x + 1));
                if (cur_coords.x + 2 == data->size_x) {
                    increase_size_x_plus = true;
                }
            }
            if (++(*data)[cur_coords.y][cur_coords.x - 1] == 4) {
                (*ring_buffer).push(Coords(cur_coords.y, cur_coords.x - 1));
                if (cur_coords.x - 1 == 0) {
                    increase_size_x_minus = true;
                }
            }
        }

        // Increase size
        uint16_t shift_x = increase_size_x_minus ? EXPANSION_DEFAULT_SIZE : 0;
        uint16_t shift_y = increase_size_y_minus ? EXPANSION_DEFAULT_SIZE : 0;
        if (increase_size_y_plus) data->increaseSizeY(0);
        if (increase_size_y_minus) data->increaseSizeY(EXPANSION_DEFAULT_SIZE);
        if (increase_size_x_plus) data->increaseSizeX(0);
        if (increase_size_x_minus) data->increaseSizeX(EXPANSION_DEFAULT_SIZE);
        shiftIndexInRingBuffer(ring_buffer, shift_x, shift_y);
        // Save .bmp if need
        if (iteration % CLI_data.freq == 0 && CLI_data.freq != 0) {
            char* filename = generateFileName(CLI_data.output, iteration);
            saveBMP(filename, data);
            delete[] filename;
            std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed =
                std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
            start = std::chrono::high_resolution_clock::now();
            std::cout << "\t:---------:\tIterations per second: ~"
                      << (iteration - iterations_start + 1) / elapsed.count() << "\n";
            iterations_start = iteration + 1;
        }
        ++iteration;
    }
    --iteration;
    // Save last one
    if (iteration % CLI_data.freq != 0) {
        char* filename = generateFileName(CLI_data.output, iteration);
        saveBMP(filename, data);
        delete[] filename;
    }
}