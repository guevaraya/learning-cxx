#include "../exercise.h"

// READ: 类模板 <https://zh.cppreference.com/w/cpp/language/class_template>

template<class T>
struct Tensor4D {
    unsigned int shape[4];
    T *data;

    Tensor4D(unsigned int const shape_[4], T const *data_) {
        unsigned int size = 1;
        // TODO: 填入正确的 shape 并计算 size
        for(int i = 0; i < 4; i++){
            size *=shape_[i];
            shape[i] = shape_[i];
        }
        std::cout << "size:"<< size << std::endl;
        data = new T[size*sizeof(T)];
        std::wmemcpy((wchar_t*)data, (wchar_t const*)data_, size *sizeof(T));
        std::cout << "wmemcpy:"<< size *sizeof(T)<< std::endl;
    }
    ~Tensor4D() {
        delete[] data;
    }

    // 为了保持简单，禁止复制和移动
    Tensor4D(Tensor4D const &) = delete;
    Tensor4D(Tensor4D &&) noexcept = delete;

    // 这个加法需要支持“单向广播”。
    // 具体来说，`others` 可以具有与 `this` 不同的形状，形状不同的维度长度必须为 1。
    // `others` 长度为 1 但 `this` 长度不为 1 的维度将发生广播计算。
    // 例如，`this` 形状为 `[1, 2, 3, 4]`，`others` 形状为 `[1, 2, 1, 4]`，
    // 则 `this` 与 `others` 相加时，3 个形状为 `[1, 2, 1, 4]` 的子张量各自与 `others` 对应项相加。
    Tensor4D &operator+=(Tensor4D const &others) {
        // TODO: 实现单向广播的加法
        /*  维度扩张 小维度补全  */

        unsigned int i, j, m, n,new_size = 1, size = 1;
        unsigned int i_index, j_index, m_index;
        std::cout << "start plus:" << shape[0] << std::endl;
        unsigned int new_shape[4];
        T *new_data;

        for(i = 0; i <4 ;i++)
        {
            size = size *others.shape[i];
            std::cout << "shape["<< i << "]:   \t" <<others.shape[i] ;
            if(shape[i] > 1 && others.shape[i] == 1)
            {
                new_shape[i] = shape[i];
                
            }else{
                new_shape[i] = others.shape[i];
                
            }
            std::cout << "\t ==> " << new_shape[i] << std::endl;
            new_size = new_size* new_shape[i];
        }
        std::cout << "new_size:" << new_size << std::endl;

        if(new_size != size){
            #if 1
            std::cout << new_size << std::endl;
            new_data = new T[new_size* sizeof(T)];
            std::cout << "new_shape[0]" << new_shape[0] << std::endl;
            std::cout << "new_shape[1]" << new_shape[1] << std::endl;
            std::cout << "new_shape[2]" << new_shape[2] << std::endl;
            std::cout << "new_shape[3]" << new_shape[3] << std::endl;
            unsigned int new_data_position = 0, data_position = 0;
             for( i = 0; i < new_shape[0]; i++){
                if(others.shape[0] != new_shape[0]){
                    i_index = 0;
                }else{
                    i_index = i;
                }

                for( j = 0; j < new_shape[1]; j++){
                    if(others.shape[1] != new_shape[1]){
                        j_index = 0;
                    }else{
                        j_index = j;
                    }
                    for( m = 0; m < new_shape[2]; m++){
                        if(others.shape[2] != new_shape[2]){
                            m_index = 0;
                        }else{
                            m_index = m;
                        }                        
                        for( n = 0; n < new_shape[3]; n++){
                            data_position = (i_index * others.shape[1]* others.shape[2]*others.shape[3]) + 
                                                 (j_index * others.shape[2]*others.shape[3]) + 
                                                  (m_index*others.shape[3]);
                            if(others.shape[3] != new_shape[3]){
                                new_data[new_data_position] = others.data[data_position];
                                data[new_data_position] += new_data[new_data_position] ;
                            }else{
                                new_data[new_data_position] = others.data[data_position+n];
                                data[new_data_position] += new_data[new_data_position] ;
                            }
                     //       std::cout << " " << new_data[new_data_position];
                            new_data_position++;
                        }
                   //     std::cout << "     m ="<< m << "\n "<< std::endl;
                    }
                 //   std::cout << " j = "<< j << "\n "<< std::endl;
                }
               // std::cout << " i= "<< i << "\n "<< std::endl;
            }
            delete [] new_data;
            #endif
        }else{
            for(i=0; i < size;i++){
               data[i] += others.data[i];
            }
        }
        
        return *this;

    }

};

// ---- 不要修改以下代码 ----
int main(int argc, char **argv) {
    {
        unsigned int shape[]{1, 2, 3, 4};
        // clang-format off
        int data[]{
             1,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,

            13, 14, 15, 16,
            17, 18, 19, 20,
            21, 22, 23, 24};
        // clang-format on
        auto t0 = Tensor4D(shape, data);
        auto t1 = Tensor4D(shape, data);
        t0 += t1;
        
        for (auto i = 0u; i < sizeof(data) / sizeof(*data); ++i) {
            std::cout << "Tensor check "<< i << "  "<< t0.data[i] << " data[i]= " << data[i] << std::endl;
            ASSERT(t0.data[i] == data[i] * 2, "Tensor doubled by plus its self.");
        }
    }
    {
        unsigned int s0[]{1, 2, 3, 4};
        // clang-format off
        float d0[]{
            1, 1, 1, 1,
            2, 2, 2, 2,
            3, 3, 3, 3,

            4, 4, 4, 4,
            5, 5, 5, 5,
            6, 6, 6, 6};
        // clang-format on
        unsigned int s1[]{1, 2, 3, 1};
        // clang-format off
        float d1[]{
            6,
            5,
            4,

            3,
            2,
            1};
        // clang-format on
        std::cout << "start clang-format on init:" << d0[1] << std::endl;
        auto t0 = Tensor4D(s0, d0);
         std::cout << "start clang-format on initD1:" << d1[1] << std::endl;
        auto t1 = Tensor4D(s1, d1);
        std::cout << "start plus:" << t0.shape[0] << std::endl;
        t0 += t1;
        for (auto i = 0u; i < sizeof(d0) / sizeof(*d0); ++i) {
            std::cout << "Tensor float check "<< i << "  "<< t0.data[i] << " t1.data[i]= " << t1.data[i] << std::endl;
            ASSERT(t0.data[i] == 7.f, "Every element of t0 should be 7 after adding t1 to it.");
        }
    }
    {
        unsigned int s0[]{1, 2, 3, 4};
        // clang-format off
        double d0[]{
             1,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,

            13, 14, 15, 16,
            17, 18, 19, 20,
            21, 22, 23, 24};
        // clang-format on
        unsigned int s1[]{1, 1, 1, 1};
        double d1[]{1};

        auto t0 = Tensor4D(s0, d0);
        auto t1 = Tensor4D(s1, d1);
        t0 += t1;
        for (auto i = 0u; i < sizeof(d0) / sizeof(*d0); ++i) {
            ASSERT(t0.data[i] == d0[i] + 1, "Every element of t0 should be incremented by 1 after adding t1 to it.");
        }
    }
}
