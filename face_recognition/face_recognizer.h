#pragma once

#pragma once
#include <dlib/dnn.h>
#include <dlib/dir_nav.h>
#include <dlib/clustering.h>
#include <dlib/image_io.h>
#include <dlib/data_io.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_saver/save_jpeg.h>
#include <dlib/gui_widgets.h>

using namespace dlib;

struct TrainedFace
{
    std::string name;
    std::vector<matrix<rgb_pixel>> images;
};

class resnet_face_recognizer
{
private:
    template <template <int, template<typename>class, int, typename> class block, int N, template<typename>class BN, typename SUBNET>
    using residual = add_prev1<block<N, BN, 1, tag1<SUBNET>>>;

    template <template <int, template<typename>class, int, typename> class block, int N, template<typename>class BN, typename SUBNET>
    using residual_down = add_prev2<avg_pool<2, 2, 2, 2, skip1<tag2<block<N, BN, 2, tag1<SUBNET>>>>>>;

    template <int N, template <typename> class BN, int stride, typename SUBNET>
    using block = BN<con<N, 3, 3, 1, 1, relu<BN<con<N, 3, 3, stride, stride, SUBNET>>>>>;

    template <int N, typename SUBNET> using ares = relu<residual<block, N, affine, SUBNET>>;
    template <int N, typename SUBNET> using ares_down = relu<residual_down<block, N, affine, SUBNET>>;

    template <typename SUBNET> using alevel0 = ares_down<256, SUBNET>;
    template <typename SUBNET> using alevel1 = ares<256, ares<256, ares_down<256, SUBNET>>>;
    template <typename SUBNET> using alevel2 = ares<128, ares<128, ares_down<128, SUBNET>>>;
    template <typename SUBNET> using alevel3 = ares<64, ares<64, ares<64, ares_down<64, SUBNET>>>>;
    template <typename SUBNET> using alevel4 = ares<32, ares<32, ares<32, SUBNET>>>;

    using anet_type = loss_metric<fc_no_bias<128, avg_pool_everything<
        alevel0<
        alevel1<
        alevel2<
        alevel3<
        alevel4<
        max_pool<3, 3, 2, 2, relu<affine<con<32, 7, 7, 2, 2,
        input_rgb_image_sized<150>
        >>>>>>>>>>>>;

    frontal_face_detector _detector = get_frontal_face_detector();
    shape_predictor _sp;
    anet_type _net;

    const uint num_pictures_per_person = 3;

public:

    resnet_face_recognizer()
    {
        deserialize("shape_predictor_5_face_landmarks.dat.tar") >> _sp;
        deserialize("dlib_face_recognition_resnet_model_v1.dat.tar") >> _net;
    }

    ~resnet_face_recognizer()
    {

    }

    std::vector<full_object_detection> get_faces_chips(std::vector<matrix<rgb_pixel>>& faces, matrix<rgb_pixel> img)
    {
        std::vector<full_object_detection>shapes;
        for (auto face : _detector(img))
        {
            auto shape = _sp(img, face);
            matrix<rgb_pixel> face_chip;
            extract_image_chip(img, get_face_chip_details(shape, 150, 0.25), face_chip);
            faces.push_back(std::move(face_chip));
            shapes.push_back(std::move(shape));
        }
        return (shapes);
    }

    void save_face_to_folder(const std::string& folder_path, const std::string& person_name, const matrix<rgb_pixel>& img)
    {
        static int last_time = clock();

        if (person_name.empty())
        {
            message_box_blocking("Person name is empty", "Please type person name");
        }

        if (clock() - last_time > 100)
        {
            if ((CreateDirectoryA(folder_path.c_str(), NULL) ||
                ERROR_ALREADY_EXISTS == GetLastError()) &&
                (CreateDirectoryA((folder_path + "\\" + person_name).c_str(), NULL) ||
                    ERROR_ALREADY_EXISTS == GetLastError()))
            {
                std::string file_name = ".\\\\" + folder_path + "\\\\" + person_name + "\\\\" + std::to_string(last_time) + ".jpg";
                save_jpeg(img, file_name);
            }
            else {
                std::cout << GetLastError();
            }

            last_time = clock();
        }
    }

    void save_face_to_folder(const std::string& folder_path, const std::string& person_name, const std::vector<matrix<rgb_pixel>>& imgs)
    {
        for (auto img : imgs)
        {
            save_face_to_folder(folder_path, person_name, img);
        }
    }

    auto detect_faces(const std::string& folder_path, std::vector<matrix<rgb_pixel>>& input_faces, std::vector<std::string>& faces_names)
    {
        const int num_input_faces = input_faces.size();
        const int num_source_faces = directory(folder_path).get_dirs().size();
        matrix<rgb_pixel> img;
        std::vector<TrainedFace> sourceFaces;
        sourceFaces.reserve(num_source_faces);
        faces_names.reserve(num_input_faces);
        
        if (num_input_faces == 0)
            return faces_names;

        for (int i = 0; i < num_source_faces; i++)
        {
            TrainedFace face;
            auto subdir = directory(folder_path).get_dirs()[i];
            auto files = subdir.get_files();

            face.name = subdir.name();
            face.images.reserve(num_pictures_per_person);

            for (int j = 0; j < num_pictures_per_person; j++)
            {
                auto file = files[std::rand() % subdir.get_files().size()];
                load_image(img, file);
                input_faces.push_back(img);
                face.images.push_back(std::move(img));
            }

            sourceFaces.push_back(std::move(face));
        }

        //start face recognition
        std::vector<matrix<float, 0, 1>> face_descriptors = _net(input_faces);

        std::vector<sample_pair> edges;
        for (size_t i = 0; i < face_descriptors.size(); ++i)
        {
            for (size_t j = i; j < face_descriptors.size(); ++j)
            {
                if (length(face_descriptors[i] - face_descriptors[j]) < 0.6)
                    edges.push_back(sample_pair(i, j));
            }
        }

        std::vector<unsigned long> labels;
        const auto num_clusters = chinese_whispers(edges, labels);

        for (int i = 0; i < num_input_faces; i++)
        {
            unsigned long num_itarations = 0;
            unsigned long average_label = 0;

            const unsigned long current_label = labels[i];
            
            for (int j = num_input_faces; j < labels.size(); j++)
            {
                if (labels[j] == current_label)
                {
                    average_label += ((j - num_input_faces) / num_pictures_per_person) + 1;
                    num_itarations++;
                }
            }

            if (num_itarations != 0)
                average_label = roundf(average_label / num_itarations);

            if (average_label == 0)
            {
                faces_names.insert(faces_names.begin() + i, "");
            }
            else
            {
                faces_names.insert(faces_names.begin() + i, sourceFaces[average_label - 1].name);
            }
        }

        return faces_names;
    }
};