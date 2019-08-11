#include <chrono>
#include <iomanip>
#include <iostream>
#include <mgl2/mgl.h>
#include "../include/pathways.h"
#include "args.h"
#include "random.h"

namespace pathways {
    template <>
    struct disassembly_type<std::string> {
        using value = std::vector<Components<std::string>>;
    };

    template <>
    auto is_basic<std::string>(std::string const& str) -> bool {
        return std::size(str) == 1;
    }

    template <>
    auto is_below<std::string>(std::string const& x, std::string const& y) -> bool {
        return y.find(x) != std::string::npos;
    }

    template <>
    auto disassemble<std::string>(std::string const& str) -> std::vector<Components<std::string>> {
        if (str.empty()) {
            throw std::invalid_argument("string is empty");
        }
        auto parts = std::vector<Components<std::string>>{};
        for (size_t i = 1, len = std::size(str); i < len; ++i) {
            parts.emplace_back(str.substr(0, i), str.substr(i));
        }
        return parts;
    }
}

struct Range {
    double min;
    double max;
};

struct PlotData {
    mglData x;
    mglData y;
    mglData err;

    std::string title;
    std::string xlabel;
    std::string ylabel;

    Range xrange;

    PlotData(long size, std::string title, std::string xlabel, std::string ylabel, Range xrange):
        x{size}, y{size}, err{size},
        title{std::move(title)}, xlabel{std::move(xlabel)}, ylabel{std::move(ylabel)},
        xrange{std::move(xrange)} {}
};

auto plot(mglGraph &gr, PlotData const& data) -> mglGraph& {
    gr.Title(data.title.c_str());
    gr.SetRange('x', data.xrange.min, data.xrange.max);
    gr.SetRange('y', data.y.Minimal(), data.y.Maximal());
    gr.Axis();
    gr.Label('x', data.xlabel.c_str());
    gr.Label('y', data.ylabel.c_str());
    gr.Error(data.x, data.y, data.err);
    gr.Plot(data.x, data.y, "b #o");

    return gr;
}

auto entropy(std::string const& str) {
    auto p = 0.0;
    for (auto const& c: str) {
        p += (c - 48);
    }
    p /= std::size(str);

    return (p == 0 || p == 1) ? 0.0 : (-p * log2(p) - (1-p) * log2(1-p));
}

template <typename Generator>
auto entropy(mglGraph &gr, Generator &gen, size_t len, size_t num_str, double p_step) -> mglGraph& {
    std::size_t const n_step = 1 + std::floor(1 / p_step);
    auto data = PlotData{
        static_cast<long>(num_str * n_step),
        "Assembly Index vs. Entropy",
        "entropy",
        "assembly index",
        { 0, 1 }
    };

    double p = 0.0;
    size_t j = 0;
    for (std::size_t i = 0; i < n_step; ++i, p += p_step) {
        for (std::size_t n = 0; n < num_str; ++n, ++j) {
            pathways::Context<std::string> ctx;
            auto const str = random_string(len, gen, p);
            data.x.a[j] = entropy(str);
            data.y.a[j] = ctx.assembly_index(str);
        }
    }

    return plot(gr, data);
}

auto main(int argc, char **argv) -> int {
    Args args{argc, argv};

    std::mt19937 gen(args.seed);

    mglGraph gr;
    gr.SuppressWarn(true);
    entropy(gr, gen, 100, args.n, 0.01);
    gr.WriteFrame(args.filename.c_str());
}
