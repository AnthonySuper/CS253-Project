#include <image_dataset.hpp>

std::unique_ptr<ImageDataset> ImageDataset::fromFile(FileBuff &buff) {
    std::vector<std::tuple<const char *, size_t>> names;
    char *ptr = buff.begin;
    char *chase = ptr;
    while(ptr != buff.end) {
        ptr++;
        if(*ptr == ' ' || *ptr == '\n' || *ptr == '\t' || *ptr == '\r') {
            if(chase != ptr) {
                *ptr = '\0';
                names.emplace_back(chase, ptr - chase - 1);
            }
            chase = ptr + 1;
        }
    }
    if(chase != ptr) {
        *ptr = '\0';
        names.emplace_back(chase, ptr - chase - 1);
    }
    std::mutex dsmut;
    std::mutex counterMut;
    std::vector<std::thread> threads;
    std::atomic<int> ind(0);
    const int max = names.size();
    auto ds = std::make_unique<ImageDataset>(names.size());
    auto buffPtr = ds->imageBuffer;
    auto hc = std::thread::hardware_concurrency();
    for(unsigned int i = 0; i < hc; ++i) {
        threads.emplace_back([&] {
                    FileBuff fb;
                    for(int a = 0; a < max;){
                        counterMut.lock();
                        a = ind;
                        ind++;
                        counterMut.unlock();
                      if(a < max) {
                          auto tuple  = names[a];
                          auto name = std::get<0>(tuple);
                          auto size = std::get<1>(tuple);
                          auto pl = buffPtr + a;
                          auto di = new(pl) DepthImage(name, size, fb);
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
        for(unsigned int i = 0; i < _size; ++i) {
            (imageBuffer + i)->~DepthImage();
        }
        std::free(imageBuffer);
    }
}

std::ostream& operator<<(std::ostream&os, ImageDataset& is) {
    for(auto& img : is) {
        os << img.getFileName() << '\t';
    }
    return os;
}
