#include <image_dataset.hpp>

ImageDataset ImageDataset::fromFile(FileBuff &buff) {
    ImageDataset ds;
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
    ds.reserve(names.size());
    std::mutex dsmut;
    std::vector<std::thread> threads;
    std::atomic<int> ind(0);
    const int max = names.size();
    auto order = std::memory_order::memory_order_relaxed;
    for(int i = 0; i < std::thread::hardware_concurrency(); ++i) {
        threads.emplace_back([&] {
                    FileBuff fb;
                    for(int a = 0; a < max; a = ind.fetch_add(1, order)){
                      if(a < max) {
                          auto tuple  = names[a];
                          auto name = std::get<0>(tuple);
                          auto size = std::get<1>(tuple);
                          auto ptr = std::make_shared<DepthImage>(name,
                                                                  size,
                                                                  fb);
                          std::lock_guard<std::mutex> lock(dsmut);
                          ds.emplace_back(ptr);
                      }
                    }
                });
    }
    for(auto& t: threads) {
        t.join();
    }
    return ds;
}

void ImageDataset::emplace_back(ImagePtr ip) {
    images.emplace_back(ip);
    addClass(images.at(images.size() - 1)->getCategory());
}

void ImageDataset::emplace_back(const DepthImage& img) {
    images.emplace_back(std::make_shared<DepthImage>(img));
    addClass(images.at(images.size() - 1)->getCategory());
}

std::shared_ptr<DepthImage>ImageDataset::at(int index) {
    return images.at(index);
}

void ImageDataset::addClass(int k) {
    classList.insert(k);
}
