# Score keeper
# William Gregory

# Python 2.7

# - settings -
sc_current_version = 0.1
sc_first_player_out = False  # first player out. record scores in order starting here
# - settings end -

def sc_setup():
    global number_players
    print("")
    print("Score Keeper v." + str(sc_current_version))
    # number of players
    try:
        number_players = int(raw_input("Enter number of players: "))
    except:
        print("Error: input not recognised")
    # players
    print("")
    print("Setup players:")
    for x in range(number_players):
        try:
            temp_name = raw_input("Enter player " + str(x) + " name: ")
            player_scores.append(0)
            players.append(temp_name)
            player_scores.append(0)
        except:
            print("Error: input not recognised")

def sc_main_loop():
    global player_scores
    print("")
    print("Begin Score Keeping")
    print("--------------------------")
    print("")
    # begin score keeping loop
    score_keeping = True
    score_round = 1
    while score_keeping:
        print("record round " + str(score_round))
        print("---")
        for x in range(number_players):
            try:
                temp_score = int(raw_input("Enter score for player " + players[x] + ": "))
                player_scores[x] = player_scores[x] + temp_score
            except:
                print("Error: input not recognised")
        print("")
        for x in range(number_players):
            print("player: " + players[x] + "\tscore: " + str(player_scores[x]))
        score_round += 1
        print("")

# autorun
number_players = 0
players = []
player_scores = []

sc_setup()
sc_main_loop()
