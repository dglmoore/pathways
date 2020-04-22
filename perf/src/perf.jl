using CSV, DataFrames, Statistics

sa(str::AbstractString) = let (c, time) = readlines(`../bin/sa $str`)
    parse(Int64, c), parse(Float64, time)
end

df = CSV.File("data/str_sa.csv") |> DataFrame
select!(df, :string, :nLetters, :nSeqs, :SA, :nPaths, :runtime)
rename!(df, Dict(:runtime => :runtime_sa))

@time asa = sa.(df.string)

df.ASA = first.(asa)
df.runtime_asa = last.(asa)

δ = df.SA - df.ASA
@show extrema(δ)
@show mean(δ) std(δ)

CSV.write("data/sa_asa.csv", df)
