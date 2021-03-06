#include "args.h"
#include "random.h"
#include <mgl2/mgl.h>
#include <pathways/string.h>

auto statistics(std::vector<double> data) {
    double const N = std::size(data);
    auto const mean = std::accumulate(std::begin(data), std::end(data), 0.0,
            [&N](double acc, double x) {
                return acc + (x / N);
            });
    auto const variance = std::accumulate(std::begin(data), std::end(data), 0.0,
            [&mean, &N](double acc, double x) {
                return acc + ((x - mean)*(x - mean))/(N - 1);
            });

    return std::make_tuple(mean, std::sqrt(variance));
}

template <typename Generator>
auto assembly_index(Generator &gen, std::size_t len, std::size_t n, double p) {
    pathways::Context<std::string> ctx;
    auto data = std::vector<double>(n);
    for (std::size_t i = 0; i < n; ++i) {
        auto const str = random_string(len, gen, p);
        data[i] = ctx.assembly_index(str);
    }
    return statistics(std::move(data));
}

struct Margins {
    double t;
    double r;
    double b;
    double l;
};

struct PlotData {
    mglData x;
    mglData y;
    mglData err;

    std::string title;
    std::string xlabel;
    std::string ylabel;

    Margins margins;

    PlotData(long size, std::string title, std::string xlabel, std::string ylabel, Margins margins):
        x{size}, y{size}, err{size},
        title{std::move(title)}, xlabel{std::move(xlabel)}, ylabel{std::move(ylabel)},
        margins{std::move(margins)} {}
};

auto plot(mglGraph &gr, PlotData const& data) -> mglGraph& {
    auto const max_err = data.err.Maximal();

    auto const x_min = data.x.Minimal() - data.margins.l;
    auto const x_max = data.x.Maximal() + data.margins.r;
    auto const y_min = data.y.Minimal() - data.margins.b - max_err;
    auto const y_max = data.y.Maximal() + data.margins.t + max_err;

    gr.Title(data.title.c_str());
    gr.SetRange('x', x_min, x_max);
    gr.SetRange('y', y_min, y_max);
    gr.Axis();
    gr.Label('x', data.xlabel.c_str());
    gr.Label('y', data.ylabel.c_str());
    gr.Error(data.x, data.y, data.err);
    gr.Plot(data.x, data.y, " #o");

    return gr;
}

template <typename Generator>
auto length_scaling(mglGraph &gr, Generator &gen, size_t min_len, size_t max_len, size_t num_str, double p) -> mglGraph& {
    auto data = PlotData{
        static_cast<long>(max_len - min_len + 1),
        "Length Scaling",
        "length",
        "assembly index",
        { 0, 1, 0, 1 }
    };

    for (std::size_t len = min_len, i = 0; len <= max_len; ++len, ++i) {
        data.x.a[i] = static_cast<double>(len);
        std::tie(data.y.a[i], data.err.a[i]) = assembly_index(gen, len, num_str, p);
    }

    return plot(gr, data);
}

template <typename Generator>
auto prob_scaling(mglGraph &gr, Generator &gen, size_t len, size_t num_str, double p_step) -> mglGraph& {
    std::size_t const n_step = 1 + std::floor(1 / p_step);
    auto data = PlotData{
        static_cast<long>(n_step),
        "Probability Scaling",
        "probability",
        "assembly index",
        { 0, 0.1, 0, 0.1 }
    };

    double p = 0.0;
    for (std::size_t i = 0; i < n_step; ++i, p += p_step) {
        data.x.a[i] = p;
        std::tie(data.y.a[i], data.err.a[i]) = assembly_index(gen, len, num_str, p);
    }

    return plot(gr, data);
}

auto main(int argc, char **argv) -> int {
    auto args = Args{argc, argv};

    std::mt19937 gen(args.seed);

    mglGraph gr;
    gr.SetSize(800, 1200);

    gr.SubPlot(1, 2, 0);
    length_scaling(gr, gen, 10, 50, args.n, 0.5);

    gr.SubPlot(1, 2, 1);
    prob_scaling(gr, gen, 50, args.n, 0.01);

    gr.WriteFrame(args.filename.c_str());
}
