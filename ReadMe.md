Of course. The issue you're seeing is due to incorrect markdown formatting. I've fixed the syntax to ensure the headings, lists, code blocks, and table are rendered correctly.

# Merkelmain: A C++ Cryptocurrency Trading Simulator

Merklemain is a command-line cryptocurrency trading application written in C++. It simulates a real-world trading environment by reading historical market data from CSV files. Users can analyze the market, manage a wallet, place buy (bid) and sell (ask) orders, and view market statistics across different timeframes. The application includes advanced features like candlestick chart generation and plotting directly in the terminal.

-----

## Features ✨

  * **Interactive Command-Line Interface**: Navigate through a user-friendly menu to interact with the trading simulation.
  * **Data-Driven Simulation**: Reads real market data from CSV files to provide a realistic trading experience.
  * **Market Analysis**: View exchange statistics for various cryptocurrency pairs, including the number of asks, and maximum and minimum ask prices.
  * **Wallet Management**: Start with a pre-funded wallet and track your assets as you trade. The system ensures you have sufficient funds before placing an order.
  * **Order Placement**: Place both ask (sell) and bid (buy) orders for different cryptocurrency pairs.
  * **Matching Engine**: The simulation automatically matches compatible bid and ask orders at each timeframe to execute sales.
  * **Candlestick Charting**:
      * Generate candlestick data (open, high, low, close) for any product and order type (bid/ask).
      * Display a candlestick plot directly in the terminal, providing a visual representation of market trends.

-----

## Data Source 💾

The application requires historical market data in CSV format to run the simulation. The project includes the following example data files:

  * `20200601.csv`
  * `20200317.csv`
  * `SOL-USD.csv`

The simulator can be configured to use different data files by changing the filename loaded in the `MerkelMain.h` constructor.

-----

## How to Compile and Run 🏃

### Prerequisites

  * A C++ compiler (like G++)
  * All project source files (`.cpp` and `.h`) and at least one data CSV file in the same directory.

### Compilation

Open your terminal, navigate to the project directory, and compile all C++ source files using the following command:

```bash
g++ *.cpp -o main
```

### Execution

Once compiled, run the executable:

```bash
# On Linux or macOS
./main

# On Windows
main.exe
```

You will be greeted by the main menu, which will look like this:

```
1: Print help 
2: Print exchange stats
3: Make an offer 
4: Make a bid 
5: Print wallet 
6: Continue 
7: Print Candlestick table 
8: Print Candlestick Plot 
9: Print Other Candlestick Plot 
============== 
Current time is: 2020/06/01 11:51:01
```

-----

## Code Structure 📂

| File                  | Description                                                                                                      |
| :-------------------- | :--------------------------------------------------------------------------------------------------------------- |
| **main.cpp** | The main entry point for the application. It creates an instance of the `MerkelMain` class and starts the simulation. |
| **MerkelMain.cpp/.h** | The main application driver. Handles user input, the main loop, and orchestrates calls to other classes.           |
| **OrderBook.cpp/.h** | Manages the collection of all orders from the dataset. Contains logic for matching bids/asks and calculating candlestick data. |
| **OrderBookEntry.cpp/.h**| Represents a single order (a bid or an ask) in the order book.                                                     |
| **Wallet.cpp/.h** | Manages the user's currency balances, ensuring they can fulfill the orders they place.                           |
| **CSVReader.cpp/.h** | A utility class for parsing data from `.csv` files into `OrderBookEntry` objects.                                  |
| **Candlestick.h** | Defines the `Candlestick` class, which stores the open, high, low, and close price data for a specific timeframe. |