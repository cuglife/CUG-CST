import random
import sys
import operator
import math
import time

#GA or PBIL
ALGORITHM_TYPE = sys.argv[8]



# **** GENETIC ALGORITHM BEGIN ****

# Only the genetic algorithm will use global
# variables from the command line.
# Command line arguments are as follows:
# 1. Name of file containing the problem
# 2. Number of individuals in the population
# 3. Method for selection: rs (rank), ts (tournament), bs (Boltzmann)
# 4. Method for crossover: 1c (1-point), uc (uniform)
# 5. Crossover probability
# 6. Mutation probability
# 7. Number of generations to run
# 8. Algorithm selection -- g to run this algorithm

#Globals from command line for GA:
FILENAME = sys.argv[1]          #file name always
N = int(sys.argv[2])            #population size always
THREE = sys.argv[3]             #see above
FOUR = sys.argv[4]              #see above
FIVE = float(sys.argv[5])       #see above
SIX = float(sys.argv[6])        #see above
GENERATIONS = int(sys.argv[7])  #generations always

#For tournament selection in GA
M = 2 #Size of tournament pool
k = 1 #Number of winners per tournament


#Opens file and extracts and returns the MAXSAT problem, number of variables, and number
#of clauses. 
def read_in_file_GA(file_name):
    test_object = open(file_name, 'r')
    data = test_object.readlines()
    out = []
    firstline = data[0].split(' ')
    num_var = firstline[2]
    num_clauses  = firstline[3]
    for line in range(1, len(data)):
        out.append(data[line].split(' ')[:-1])
        #data[line]= data[:-1]


    test_object.close()

    return (out, int(num_var), int(num_clauses))


#Globals from file:
PROBLEM, NUM_VAR, NUM_CLAUSES = read_in_file_GA(FILENAME)

def generate_individual():
    #Generates one random instance of an individual or solution
    #returns one random individual in array form

    #this array will represent one individual in our population
    individual = []

    #randomly select a value (T or F)
    #individual should have as many values as there are variables
    for num in range(int(NUM_VAR)):
        individual.append(random.choice([True, False]))


    return individual



def generate_population():
    #uses generate individual function to create a population
    #of size N. Returns an array of the population

    #population will be an array of individuals (array of arrays)
    population = []

    #generate individual N times and add to array
    for num in range(N):
        population.append(generate_individual())



    return population


def evaluate_fitness(population):

    #Evaluates how many clauses can be satisfied for each individual
    #in the population. Returns array of scores. 

    #array of clause counts for population
    population_clauses = []

    #loop through population
    for i in range(N):

        #count for this individual
        clause_count = 0

        #loop through clauses
        for clause in PROBLEM:
            
            #loop through variables
            for var in clause:
                
                #if variable is negative
                if int(var) < 0:

                    #satisfied variable, update and go to next clause
                    #break out of this clause-- dont need to check more
                    if population[i][abs(int(var)) - 1] == False:
                        clause_count += 1
                        break

                    #unsatisfied variable, go to next variable
                    else:
                        continue

                #if variable is positive
                else:

                    #satisfied variable, update and go to next clause
                    #break out of this clause-- dont need to check more
                    if population[i][int(var) - 1] == True:
                        clause_count += 1
                        break

                    #unsatisfied variable, go to next variable
                    else:
                        continue

        #append the fitness score for each individual to this array
        population_clauses.append(clause_count)



    return population_clauses

def select_individual_rank(ranks, sum_ranks):
    #Select and return rank of one individual by rank selection


    rand_num = random.randint(1, sum_ranks)

    running_total = 0

    #loop through individuals
    for rank in ranks:

        running_total += int(rank)

        if running_total >= rand_num:
            return rank
        else:
            continue

    return -1

def ranked_selection(population, fitness):
    #Selects individuals for breeding pool using ranked selection.
    #Returns an array of individuals that represends the breeding
    #pool.


    #This section uses the zip and sort function to format arrays in
    #the necessary order.

    #combines using tuples
    new_list = zip(population, fitness)

    #sorts by fitness 
    sorted_by_second = sorted(list(new_list), key=lambda tup: tup[1])

    #Zip(*) seperates previous list. Now we have pop in order of rank
    pop, fit = zip(*sorted_by_second)

    #zip up again with an iterator to assign ranks
    good_list = zip(pop, fit ,list(range(1, N +1 )))
    
    #zip(*) into 3 lists
    pop, fit, ranks = zip(*good_list)

    #necessary
    pop_list = list(pop)
    fit_list = list(fit)
    ranks_list = list(ranks)

    #get sum of ranks for selection
    rank_sum = sum(ranks_list)


    #select_individual_rank() returns the rank of a selected individual
    #breeding_pool_ranks will store each of these. Do this N times
    breeding_pool_ranks = []
    for num in range(N):
        breeding_pool_ranks.append(select_individual_rank(ranks_list, rank_sum))


    #We want to return the individuals values in the breeding pool rather than ranks
    breeding_pool = []

    #for each rank in our breeding pool
    for rank in breeding_pool_ranks:

        #this rank val is used to grab correct individual
        breeding_pool.append(pop_list[rank - 1])




    return breeding_pool

def tournament_selection(population, fitness):
    #this function performs tournament selection
    #each tournament consists of M individuals and k winners
    #M and k are globals
    #returns a list of the winners (breeding pool)

    breeding_pool = []

    #We need N in our breeding pool, so we will do 
    #(N/k) tournaments
    for i in range(int((N/k))):



        #Randomly select M from population for tourn
        tournament_pool = []

        #select M individuals for tournament
        for j in range(M):

            rand_num = random.randint(0, N-1)
            tournament_pool.append((population[rand_num], fitness[rand_num]))

        #Do tournament --- compare
        winner = max(tournament_pool,key=operator.itemgetter(1))[0] 

        #add winner to breeding pool
        breeding_pool.append(winner)



    return breeding_pool

def Boltzmann_select_one(b_fitnesses, b_sum):
    #Selects one individual using Boltzmann probability
    #Returns index of selected individual

    #generates a random value between 0 and the boltzmann sum
    rand_num = random.uniform(0, b_sum)

    #this value will be updated with each boltzmann fitness
    #and compared to the random number
    running_total = 0

    #This number will represent the index of which individual 
    #we are at. 
    index_counter = 0

    #For each individual, add the Boltzmann probability to 
    #the running_total and perform check. 
    for fit in b_fitnesses:

        running_total += fit

        #if true, return index of individual we are at
        if running_total >= rand_num:
            return index_counter


        #go to next individual and update index
        else:
            index_counter += 1
            continue




    return 100

def Boltzmann_selection(population, fitness):


    #need to get boltzmann fitness and sum 
    boltzmann_fitnesses = []

    #loop through fitness
    for fit in fitness:

        #e^fitness
        b_fitness = math.exp(fit)

        boltzmann_fitnesses.append(b_fitness)

    #get sum for selection
    boltzmann_sum = sum(boltzmann_fitnesses)

    #initialize breeding pool
    breeding_pool_indices = []

    #select individual N times
    for num in range(N):

        #select individual
        breeding_pool_indices.append(Boltzmann_select_one(boltzmann_fitnesses, boltzmann_sum))


    #We have indices of selected individuals, we want actual individuals
    #Use indices to grab actual individuals from population. Add to BP
    breeding_pool = []

    for ind in breeding_pool_indices:
        breeding_pool.append(population[ind])


    return breeding_pool

def one_point_crossover(breeding_pool):
    #Performs 1-point crossover with probability
    #specified as command line argument. 


    #count represents how many children we have created
    count = 0
    
    #return this (population to send to mutation)
    new_pop = []


    #do crossover until we have enough children 
    while count < N:



        #select two parents from breeding pool randomly

        parent1_rand = random.randint(0, N - 1)
        parent2_rand = random.randint(0, N - 1)

        #two parents that are selected
        parent1 = breeding_pool[parent1_rand]
        parent2 = breeding_pool[parent2_rand]


        #decide to do crossover with crossover probability
        rand_num = random.uniform(0.0, 1.0)

        #do crossover
        if rand_num <= FIVE:
            

            #var - 1 possibilities for points
            rand_point = random.randint(1, NUM_VAR - 1)

            #grab parts of the parent past the crossover point
            parent1_end = parent1[rand_point:]
            parent2_end = parent2[rand_point:]

            #create children
            child1 = parent1[:rand_point] + parent2_end
            child2 = parent2[:rand_point] + parent1_end

            #send children to mutation
            new_pop.append(child1)
            new_pop.append(child2)
            
            #we have created two children
            count += 2


        #dont do crossover
        else:
            #parents still sent to mutation
            new_pop.append(parent1)
            new_pop.append(parent2)

            #treat count same way as children
            count += 2
            

    #1 point creates 2 children every time
    #This makes sure we dont exceed N with our pop
    #Would happen if N is odd
    if len(new_pop) > N:
        new_pop.pop()

    return new_pop

def uniform_crossover(breeding_pool):
    #Performs uniform crossover on breeding pool
    #with probability specified as a command line
    #argument

    #count keeps track of how many individuals 
    #are ready for mutation
    count = 0

    #array of individuals that will be sent to mutation
    new_pop = []


    #Do crossover until we have N individuals to send to mutation
    while count < N:



        #select two parents from breeding pool

        parent1_rand = random.randint(0, N - 1)
        parent2_rand = random.randint(0, N - 1)

        #parents selected
        parent1 = breeding_pool[parent1_rand]
        parent2 = breeding_pool[parent2_rand]


        #decide to do crossover with probability
        rand_num = random.uniform(0.0, 1.0)

        #do crossover
        if rand_num <= FIVE:

            #the child we are creating
            child = []

            #loop over parents, decide which to take from with 1/2 probability
            for i in range(NUM_VAR):

                parent_decision = random.uniform(0.0, 1.0)

                if parent_decision <= 0.5:
                    child.append(parent1[i])
                    

                else:
                    child.append(parent2[i])
                    

            #add child to new pop, update count
            new_pop.append(child)
            count += 1



        #dont do crossover
        else:
            #we are sending both parents through if crossover doesnt happen
            new_pop.append(parent1)
            new_pop.append(parent2)

            count += 2
            

    
    #This makes sure we dont exceed N with our pop
    #Would happen if we need 1 more and 2 go through
    if len(new_pop) > N:
        new_pop.pop()

    return new_pop


def mutation(current_pop):

    final_pop = current_pop
    #loop through solutions in population
    for i in range(N):

        #loop through variables in a solution
        for j in range(NUM_VAR):

            
            #decide if mutation will happen with probability
            rand_num = random.uniform(0.0, 1.0)

            #do mutation
            if rand_num <= SIX:

                #flip boolean value in this place
                final_pop[i][j] = not final_pop[i][j]

            #dont do mutation, continue through loops
            else:
                continue





    return final_pop


def genetic_algorithm():


    #initialize generation counter
    generation_counter = 0

    #initialize best fitness tracker
    best_fitness = 0


    #generate our inital population
    population = generate_population()


    #Do algorithm if possible until we hit our generation limit
    while generation_counter < GENERATIONS:

        #Add a generation to our counter
        generation_counter += 1

        #evaluate fitness of population
        fitness = evaluate_fitness(population)



        #CHECK FOR OPTIMAL SOLUTION HERE
        #This line grabs the best fitness value and the index of it
        #Important because index of this corresponds to index of actual solution
        best_index, best_value = max(enumerate(fitness), key=operator.itemgetter(1))

        #If the best value we have found in this generation
        #is better than global best, update
        if best_value > best_fitness:
            best_fitness = best_value
            best_solution = population[best_index]


        #Check to see if our global best optimal yet
        #if so, we are done
        if best_fitness == NUM_CLAUSES:
            break


        #Decide Selection method
        if THREE == "rs":
            breeding_pool = ranked_selection(population, fitness)
        elif THREE == "ts":
            breeding_pool = tournament_selection(population, fitness)
        elif THREE == "bs":
            breeding_pool = Boltzmann_selection(population, fitness)

        #Decide Crossover method
        if FOUR == "1c":
            current_pop = one_point_crossover(breeding_pool)
        elif FOUR == "uc":
            current_pop = uniform_crossover(breeding_pool)

        #Mutate population
        population = mutation(current_pop)

        


    #OUTPUT SETUP

    print("The filename for this problem is: " + FILENAME)
    print("Number of variables: " + str(NUM_VAR))
    print("Number of clauses: " + str(NUM_CLAUSES))
    print("Number of clauses satisfied: " + str(best_fitness))
    print("Percentage of clauses satisfied: " + "{}%".format((best_fitness/float(NUM_CLAUSES)) * 100))
    print("Solution: " + str(best_solution))
    print("This solution was found during iteration: " + str(generation_counter))


#   **** END OF GENETIC ALGORITHM ****
    



#   **** START OF PBIL ****



def read_in_file_PBIL(file_name):
    """Given a file with a maxsat problem in specified format,
    parse the file and return an array of boolean clauses"""
    test_object = open(file_name, 'r')
    data = test_object.readlines()
    clause_set = []
    firstline = data[0].split(' ')
    num_var = firstline[2]
    num_clauses = firstline[3]

    for line in range(1, len(data)):
        str_array = data[line].split(' ')[:-1]  # copy into array
        clause = list(map(int, str_array))  # map from str to ints
        clause_set.append(clause)

    test_object.close()

    return clause_set, int(num_var), int(num_clauses)


def create_sample_vectors(vector, num_samples):
    """Create num_samples number of random samples, where each sample array
    is comprised of booleans such that the probability that sample[i] == True
    is equal to probability_vector[i]. Returns list of samples, each of which
    is a list."""
    samples = []

    for i in range(0, num_samples):
        new_sample = [False] * (len(vector))
        for j in range(0, len(vector)):
            if random.uniform(0, 1) < vector[j]:
                new_sample[j] = True
        samples.append(new_sample)

    return samples


def mutate_probability_vector(vector, mutate_prob, mutate_shift):
    """Given a probability vector, the probability of a mutation, and the
    strength of a mutation, apply mutations to create a new probability vector
    and the result."""
    new_vector = vector
    mutate_direction = 0  # default direction is downward
    for i in range(0, len(vector)):
        if random.uniform(0, 1) <= mutate_prob:
            if random.uniform(0, 1) <= .5:  # switch direction up
                mutate_direction = 1
            new_vector[i] = vector[i] * (1.0 - mutate_shift) + mutate_direction * mutate_shift
    return new_vector


def is_clause_satisfied(clause, sample):
    """Given a single clause and a single sample population,
    determine whether the sample satisfies the clause."""
    for i in range(0, len(clause)):
        index_in_sample = abs(clause[i]) - 1

        if clause[i] < 0 and not sample[index_in_sample]:  # neg num satisfied by False

            return True

        elif clause[i] > 0 and sample[index_in_sample]:  # pos num satisfied by True

            return True


    return False


def count_satisfied_clauses(clause_set, sample):
    """Given a set of boolean clauses and a single sample, determine
    how many clauses the sample answer set satisfies."""
    counter = 0
    for clause in clause_set:
        if is_clause_satisfied(clause, sample):
            counter += 1
    return counter


def find_sample_counts(clause_set, samples):
    """Given a list of clauses and a list of samples, return a list
    sample_satisfy_count such that sample_satisfy_count[i] = x
    means that samples[i] satisfied x clauses"""
    sample_satisfy_count = [None] * len(samples)
    for i in range(0, len(samples)):
        sample_satisfy_count[i] = count_satisfied_clauses(clause_set, samples[i])
    return sample_satisfy_count


def find_outlier_samples(sample_satisfy_counts, samples):
    """Finds the best and the worst samples based on the minimum and maximum.
    Returns the best sample, the worst sample, and the index of the best sample"""
    best_amount = max(sample_satisfy_counts)
    best_index = sample_satisfy_counts.index(best_amount)
    best_sample = samples[best_index]

    worst_amount = max(sample_satisfy_counts)
    worst_index = sample_satisfy_counts.index(worst_amount)
    worst_sample = samples[worst_index]

    return best_sample, worst_sample, sample_satisfy_counts[best_index]


def update_vector_with_outlier(probability_vector, best_sample, rate):
    """Updates a probability vector using the outlying best sample using
    a magnitude rate. Reusable for update-to-best and update-from-worst"""
    for i in range(0, len(probability_vector)):

        vec = probability_vector[i]

        vec = vec * (1.0 - rate) + (best_sample[i] * rate)

        if vec > 1:
            vec = 1
        elif vec < 0:
            vec = 0

        probability_vector[i] = vec

    return probability_vector


def run_pbil(input_list):

    #Command line args for PBIL
    file_name = input_list[0]
    num_individuals = int(input_list[1])
    learning_rate = float(input_list[2])
    neg_learning_rate = float(input_list[3])
    mutation_prob = float(input_list[4])
    mutation_amount = float(input_list[5])
    num_iterations = int(input_list[6])

    clause_set, num_var, num_clauses = read_in_file_PBIL(file_name)
    print("Beginning...")

    probability_vector = [.5] * num_var  # starting values
    print()
    best_clauses_sat = 0
    print("num_iterations:", num_iterations)

    for i in range(0, num_iterations):

        samples = create_sample_vectors(probability_vector, num_individuals)
        sample_satisfy_counts = find_sample_counts(clause_set, samples)
        best, worst, best_clauses_sat = find_outlier_samples(sample_satisfy_counts, samples)

        probability_vector = update_vector_with_outlier(probability_vector, best, learning_rate)
        if best != worst:
            probability_vector = update_vector_with_outlier(probability_vector, best, neg_learning_rate)
        mutate_probability_vector(probability_vector, mutation_prob, mutation_amount)



    print("The filename for this problem is: " + file_name)
    print("Number of variables: " + str(num_var))
    print("Number of clauses: " + str(num_clauses))
    print("Number of clauses satisfied: " + str(best_clauses_sat))
    print("Percentage of clauses satisfied: " + "{}%".format((best_clauses_sat/float(num_clauses)) * 100))
    print("Solution: " + str(probability_vector))
    print("This solution was found during iteration: " + str(i+1))

#   **** END OF PBIL ****





def main():
    input_list = sys.argv[1:]  # ignore 'main.py' command
    # print(input_list)
    if len(input_list) != 8:
        print("Error... must accept 8 command line arguments")
        return
    start_time = time.time()
    if ALGORITHM_TYPE == 'p':
        run_pbil(input_list)

    if ALGORITHM_TYPE == 'g':
        genetic_algorithm()
    print("The Program took: " + str(time.time() - start_time) + " seconds")
if __name__ == '__main__':
    main()
