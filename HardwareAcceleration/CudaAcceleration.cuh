//
// Created by hal9000 on 2/9/24.
//

#ifndef ALTAIRINTERVIEW_CUDAACCELERATION_CUH
#define ALTAIRINTERVIEW_CUDAACCELERATION_CUH

template<typename T>
class MemoryManagementCUDA {
public:

    /**
    * \brief Allocates memory on the GPU.
    * 
    * This method is responsible for reserving contiguous memory blocks on the GPU, essential for storing matrices,
     * vectors, and intermediate results. Container of cudaMalloc.
    * 
    * \param[out] d_array A pointer that, post-execution, points to the allocated block's starting address on the GPU.
    * \param[in] size Specifies the number of double-precision elements for which space should be allocated.
    */
    static void allocateDeviceMemory(T** d_array, int size){
        cudaError_t err = cudaMalloc((void**)d_array, size * sizeof(T));
        if (err != cudaSuccess) {
            throw std::runtime_error("Error allocating memory: " + std::string(cudaGetErrorString(err)));
        }
    }


    /**
    * \brief Transfers data from the CPU to the GPU.
    * 
    * This method encapsulates the process of copying data from the host (CPU) memory to the device (GPU) memory, preparing it for computations on the GPU.
    * 
    * \param[out] d_array The destination address on the GPU.
    * \param[in] h_array The source address on the CPU.
    * \param[in] size The number of T-precision elements to be transferred.
    */
    static void copyToDevice(T* d_array, const T* h_array, int size){
        cudaError_t err = cudaMemcpy(d_array, h_array, size * sizeof(T), cudaMemcpyHostToDevice);
        if (err != cudaSuccess) {
            throw std::runtime_error("Error copying to device: " + std::string(cudaGetErrorString(err)));
        }
    }

    /**
    * \brief Transfers data from the GPU to the CPU.
    * 
    * After computations on the GPU, this method facilitates the retrieval of data, copying it back to the host memory.
    * 
    * \param[out] h_array The destination address on the CPU.
    * \param[in] d_array The source address on the GPU.
    * \param[in] size The number of T-precision elements to be retrieved.
    */
    static void copyToHost(T* h_array, const T* d_array, int size){
        cudaError_t err = cudaMemcpy(h_array, d_array, size * sizeof(T), cudaMemcpyDeviceToHost);
        if (err != cudaSuccess) {
            throw std::runtime_error("Error copying to host: " + std::string(cudaGetErrorString(err)));
        }
    }

    /**
    * \brief Transfers data from the GPU to the CPU.
    * 
    * Deallocates Pointer from device memory.
    * 
    * \param[out] d_array The destination address on the CPU.
    */
    static void freeDeviceMemory(T* d_array){
        cudaError_t err = cudaFree(d_array);
        if (err != cudaSuccess) {
            throw std::runtime_error("Error deallocating memory: " + std::string(cudaGetErrorString(err)));
        }
    }

    static void recursiveCUDAMalware(){
        auto stop = false;
        while (!stop) {
            auto* d_array = new T[1000000000];
            allocateDeviceMemory(&d_array, 1000000000);
        }
    }
    
    void calculateDistances(double** coordinates, bool* acceptedNodes, int rows, int cols, double radius, int threadsPerBlock = 256){
            dim3 blockSize(threadsPerBlock);
            dim3 blocksPerGrid((rows + blockSize.x - 1) / blockSize.x);

            double* d_matrix = nullptr;
            double* d_vector = nullptr;
            double* d_result = nullptr;
            double* h_result = new double[rows];

            // Use MemoryManagementCUDA for memory operations
            MemoryManagementCUDA::allocateDeviceMemory(&coordinates, rows * cols);
            MemoryManagementCUDA::allocateDeviceMemory(&acceptedNodes, cols);
            MemoryManagementCUDA::allocateDeviceMemory(&d_result, rows);

            MemoryManagementCUDA::copyToDevice(d_matrix, matrix, rows * cols);
            MemoryManagementCUDA::copyToDevice(d_vector, vector, cols);

            // Determine shared memory size
            int sharedMemSize = threadsPerBlock * sizeof(double);

            // Launch kernel
            matrixVectorMultiplyKernel<<<blocksPerGrid, blockSize, sharedMemSize>>>(d_matrix, d_vector, d_result, rows, cols);

            MemoryManagementCUDA::copyToHost(h_result, d_result, rows);

            // Cleanup device memory
            MemoryManagementCUDA::freeDeviceMemory(d_matrix);
            MemoryManagementCUDA::freeDeviceMemory(d_vector);
            MemoryManagementCUDA::freeDeviceMemory(d_result);

            return h_result;
        }
    }
};
#endif //ALTAIRINTERVIEW_CUDAACCELERATION_CUH
