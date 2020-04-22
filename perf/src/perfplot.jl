using CSV, DataFrames, Statistics, StatsPlots

if !isdir("plots")
    mkdir("plots")
end

df = CSV.File("data/sa_asa.csv") |> DataFrame

df.δSA = df.SA - df.ASA
df.δtime = df.runtime_sa - df.runtime_asa

@df df scatter(:nLetters, :ASA, xlabel="nLetters", ylabel="ASA", label="", zcolor=:runtime_asa, markercolors=:blues)
savefig("plots/asa_vs_length.png")

@df df scatter(:nLetters, :runtime_asa, xlabel="nLetters", ylabel="runtime", label="", zcolor=:runtime_asa, markercolors=:blues)
savefig("plots/runtime_vs_length.png")

gf = by(df, :nLetters, [:δSA, :δtime] => x -> (δSA = mean(x.δSA), δt = mean(x.δtime)))
@df gf scatter(:nLetters, :δSA, xlabel="nLetters", ylabel="mean error", label="", zcolor=:δt, markercolor=:blues)
savefig("plots/mean-error_vs_length.png")

gf = by(df, :nLetters, [:SA, :δSA, :δtime] => x -> (δSA = mean(x.δSA ./ x.SA), δt = mean(x.δtime)))
@df gf scatter(:nLetters, :δSA, xlabel="nLetters", ylabel="mean percent error", label="", zcolor=:δt, markercolor=:blues)
savefig("plots/mean-percent-error_vs_length.png")
