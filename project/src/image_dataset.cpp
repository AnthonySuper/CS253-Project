#include <image_dataset.hpp>

std::unique_ptr<ImageDataset> ImageDataset::fromFile(FileBuff &buff) {
    std::vector<std::tuple<const char *, size_t>> names;
    char *ptr = buff.begin;
    char *chase = ptr;
    while(ptr != buff.end) {
        ptr++;
        if(*ptr == ' ' || *ptr == '\n' || *ptr == '\t') {
            if(chase != ptr) {
                *ptr = '\0';
                names.emplace_back(chase, ptr - chase - 1);
            }
        }
    }
    if(chase != ptr) {
        *ptr = '\0';
        names.emplace_back(chase, ptr - chase - 1);
    }
    
    std::mutex dsmut;
    std::vector<std::thread> threads;
    std::atomic<int> ind(0);
    const int max = names.size();
    auto order = std::memory_order::memory_order_relaxed;
    auto ds = std::make_unique<ImageDataset>(names.size());
    auto buffPtr = ds->imageBuffer;
    for(int i = 0; i < std::thread::hardware_concurrency(); ++i) {
        threads.emplace_back([&] {
                    FileBuff fb;
                    for(int a = 0; a < max; a = ind.fetch_add(1, order)){
                      if(a < max) {
                          auto tuple  = names[a];
                          auto name = std::get<0>(tuple);
                          auto size = std::get<1>(tuple);
                          auto di = new(buffPtr + a) DepthImage(name, size, fb);
                          std::lock_guard<std::mutex> guard(dsmut);
                          ds->addClass(di->getCategory());
                      }
                    }
                });
    }
    for(auto& t: threads) {
        t.join();
    }
    return ds;
}


ImageDataset::ImageDataset(ImageDataset && id) :
_size(id._size),
imageBuffer(id.imageBuffer),
classList(std::move(id.classList))
{
    id._size = 0;
    id.imageBuffer = nullptr;
}

void ImageDataset::addClass(int k) {
    classList.insert(k);
}

ImageDataset::~ImageDataset() {
    if(imageBuffer != nullptr) {
        for(int i = 0; i < _size; ++i) {
            (imageBuffer + i)->~DepthImage();
        }
        std::free(imageBuffer);
    }
}
