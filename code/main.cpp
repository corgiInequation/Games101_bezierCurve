#include <chrono>
#include <iostream>
#include <opencv2/opencv.hpp>

std::vector<cv::Point2f> control_points;

void mouse_handler(int event, int x, int y, int flags, void *userdata) 
{
    if (event == cv::EVENT_LBUTTONDOWN && control_points.size() < 4) 
    {
        std::cout << "Left button of the mouse is clicked - position (" << x << ", "
        << y << ")" << '\n';
        control_points.emplace_back(x, y);
    }     
}

void naive_bezier(const std::vector<cv::Point2f> &points, cv::Mat &window) 
{
    auto &p_0 = points[0];
    auto &p_1 = points[1];
    auto &p_2 = points[2];
    auto &p_3 = points[3];

    for (double t = 0.0; t <= 1.0; t += 0.001) 
    {
        auto point = std::pow(1 - t, 3) * p_0 + 3 * t * std::pow(1 - t, 2) * p_1 +
                 3 * std::pow(t, 2) * (1 - t) * p_2 + std::pow(t, 3) * p_3;

        window.at<cv::Vec3b>(point.y, point.x)[2] = 255;
    }
}

cv::Point2f recursive_bezier(const std::vector<cv::Point2f> &control_points, float t) 
{
    int len = control_points.size();
    if(len == 1)
        return control_points[0];
    std::vector<cv::Point2f> next_control_points;
    for(int i = 0; i < len - 1; i++)
    {
        next_control_points.push_back((1-t)*control_points[i] + t*control_points[i+1]);
    } 

    return recursive_bezier(next_control_points, t);
    

}

void bezier_better(const std::vector<cv::Point2f> &control_points, cv::Mat &window) 
{
    // TODO: Iterate through all t = 0 to t = 1 with small steps, and call de Casteljau's 
    // recursive Bezier algorithm.
    //想要反走样，直接
    for(double t = 0.001; t <= 1.0; t+=0.001)
    {
        cv::Point2f nowPoint = recursive_bezier(control_points, t);
        
        float x = nowPoint.x;
        float y = nowPoint.y;
        cv::Point2f plt;
        plt.x = x - std::floor(x) > 0.5 ? std::ceil(x) - 0.5 : std::floor(x) - 0.5;
        plt.y = y - std::floor(y) > 0.5 ? std::ceil(y) - 0.5 : std::floor(y) - 0.5;
        std::vector<cv::Point2f> ps = {plt, cv::Point2f(plt.x + 0.5f, plt.y), cv::Point2f(plt.x, plt.y - 0.5f), cv::Point2f(plt.x + 0.5f, plt.y + 0.5f)};
        for(int i = 0; i < ps.size(); i++)
        {
            float dis = cv::norm(nowPoint - ps[i]);
            window.at<cv::Vec3b>(ps[i].y, ps[i].x)[1] = std::clamp((float)(0.8*255.0f*(std::sqrt(2) - dis)/std::sqrt(2) + window.at<cv::Vec3b>(ps[i].y, ps[i].x)[1]), 0.0f, 255.0f);
        }
    }

    int width  = window.cols;
    int height = window.rows;
    int w[] = {0,-1,0,1,0,-1, 1, 1, -1};
    int h[] = {0,0,1,0,-1, 1, 1,-1, -1};

    //抗锯齿
    for(int i = 2; i < height - 2; i++)
    {
        for(int j = 2; j < width - 2; j++)
        {
            float color = 0;
            for(int k = 0; k < 9; k++)
            {
                cv::Point2f p = {j + w[k]*1.0f, i + h[k]*1.0f};
                color += window.at<cv::Vec3b>(p.y, p.x)[1];
            }
            window.at<cv::Vec3b>(i, j)[1] = color/9;
        }
    }

}

void bezier(const std::vector<cv::Point2f> &control_points, cv::Mat &window) 
{
    for(double t = 0.001; t <= 1.0; t+=0.001)
    {
        cv::Point2f nowPoint = recursive_bezier(control_points, t);
        window.at<cv::Vec3b>(nowPoint.y, nowPoint.x)[1] = 255;
    }
}

int main() 
{
    cv::Mat window = cv::Mat(700, 700, CV_8UC3, cv::Scalar(0));
    cv::cvtColor(window, window, cv::COLOR_BGR2RGB);
    cv::namedWindow("Bezier Curve", cv::WINDOW_AUTOSIZE);

    cv::setMouseCallback("Bezier Curve", mouse_handler, nullptr);

    int key = -1;
    while (key != 27) 
    {
        for (auto &point : control_points) 
        {
            cv::circle(window, point, 3, {255, 255, 255}, 3);
        }

        if (control_points.size() == 4) 
        {
            //naive_bezier(control_points, window);
            bezier_better(control_points, window);

            cv::imshow("Bezier Curve", window);
            cv::imwrite("my_bezier_curve.png", window);
            key = cv::waitKey(0);

            return 0;
        }

        cv::imshow("Bezier Curve", window);
        key = cv::waitKey(20);
    }

return 0;
}
