

struct StockSimulationParameters {
    double initialPrice;
    double volatility;
    double drift;
    double dt;

    StockSimulationParameters(double initialPrice, double volatility, double drift, double dt)
        :  initialPrice(initialPrice), volatility(volatility), drift(drift), dt(dt) {}
};

struct StockSimulator {
    StockSimulationParameters parameters;
    double price;

    StockSimulator(const StockSimulationParameters& params)
        : parameters(params), price(params.initialPrice) {}

    double simulate(int i) {
        double shock = parameters.volatility * std::sqrt(parameters.dt) * (std::rand() / static_cast<double>(RAND_MAX) - 0.5) * 2.0;
        double driftTerm = parameters.drift * parameters.dt * i;
        price *= std::exp((parameters.drift - 0.5 * parameters.volatility * parameters.volatility) * parameters.dt + shock + driftTerm);
        return price;
        
    }
};