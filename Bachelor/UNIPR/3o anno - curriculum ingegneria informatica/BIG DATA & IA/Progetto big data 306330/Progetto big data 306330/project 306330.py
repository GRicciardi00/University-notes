#Code by Giuseppe Ricciardi, matricola 306330 Università degli studi di Parma

#disabling an useless warning from XBOOSTING
import warnings
warnings.simplefilter(action='ignore', category=FutureWarning)

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.decomposition import PCA
from sklearn.manifold import TSNE
from sklearn.feature_selection import SelectKBest
from sklearn.feature_selection import chi2
from sklearn.feature_selection import mutual_info_classif
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LogisticRegression
from sklearn.metrics import accuracy_score
from sklearn import tree
from sklearn.ensemble import RandomForestClassifier
from sklearn.svm import SVC
from xgboost import XGBClassifier
from sklearn.ensemble import GradientBoostingClassifier
import tensorflow as tf
from tensorflow import keras
from tensorflow.keras import models
from tensorflow.keras import layers
import scipy.stats as stats
from sklearn.experimental import enable_halving_search_cv
from sklearn.model_selection import HalvingRandomSearchCV
from scipy.stats import uniform as sp_randFloat
from scipy.stats import randint as sp_randInt
from time import time
from skopt import BayesSearchCV
from skopt.plots import plot_objective, plot_histogram
from sklearn.model_selection import learning_curve
from sklearn.metrics import confusion_matrix
from sklearn.metrics import ConfusionMatrixDisplay
from keras_tuner.tuners import RandomSearch
import keras_tuner as kt

#Costants and global values
#_data analysis & feature selection
NUMERICAL_DATA = ["BMI", "MentHlth", "PhysHlth"]
FIGURE_SIZE = [15,15]
FEATURES_NUMBER = 7
#_dimensionality reduction
N_SEARCH = 10
N_COMPONENTS = 3
PERPLEXITY = 40
N_ITER = 1500

def data_analysis(dt_x, dt_y):
    digit = input('Insert 1 for general report, 0 for detailed report\n')
    if digit == '1': #general report
        plt.figure(figsize = (FIGURE_SIZE[0], FIGURE_SIZE[1]))
        plt.suptitle("GENBERAL REPORT", fontsize = 14)
        plt.subplot(6,4, 1).set_title(dt_y.name, color = 'red').figure.tight_layout(pad = 3)
        dt_y.value_counts().plot(kind = "bar", rot = 0)
        for i in range(len(dt_x.columns)):
            plt.subplot(6,4, i+2).set_title(dt_x.columns[i]).figure.tight_layout(pad = 3)
            if dt_x.columns[i] in NUMERICAL_DATA:
                plt.hist(dt_x[dt_x.columns[i]])
            else:
                dt_x[dt_x.columns[i]].value_counts().plot(kind = "bar", rot = 0)
        
        plt.show()
    elif digit == '0': #detailed report
        mediana = dt_y.median()
        mode = dt_y.value_counts().idxmax()           
        print(dt_y.name + ' is a categorical data\nHis median is ' + str(mediana) + '\nHis mode is ' + str(mode) + '\n')
        dt_y.value_counts().sort_index().plot(kind = "pie", autopct= '%1.1f%%').get_yaxis().set_visible(False)
        plt.title(dt_y.name, color = 'red')
        plt.show()
        for i in range(len(dt_x.columns)):
            if dt_x.columns[i] in NUMERICAL_DATA: #plotting histogram and calculate avg and standard deviation (std)
                plt.hist(dt_x[dt_x.columns[i]])
                plt.title(dt_x.columns[i])
                plt.xlabel('values')
                avg = round(dt_x[dt_x.columns[i]].mean(),3)
                std = round(dt_x[dt_x.columns[i]].std(),3)
                print(dt_x.columns[i] + ' is a numerical data\nHis average is ' + str(avg) + '\nHis standard deviation is ' + str(std) + '\n' )
                plt.show()
            else: #plotting pie plot and calculate median and mode
                dt_x[dt_x.columns[i]].value_counts().sort_index().plot(kind = "pie", autopct= '%1.1f%%').get_yaxis().set_visible(False)
                plt.title(dt_x.columns[i])
                median = dt_x[dt_x.columns[i]].median()
                mode = dt_x[dt_x.columns[i]].value_counts().idxmax()           
                print(dt_x.columns[i] + ' is a categorical data\nHis median is ' + str(median) + '\nHis mode is ' + str(mode) + '\n')
                plt.show()

    else:
        print('Insert a valid number! !\n')

def dimensionality_reduction(X,y):
    X_pca = X.copy()
    X_tsne = X.copy()
    print("Plotting dimensionality reduction with PCA and T-SNE")
    #3D reduction witch PCA
    pca = PCA(n_components=N_COMPONENTS) #Dimensions new hyperplane
    pca_result = pca.fit_transform(X_pca.values)
    #3D reduction with TSNE
    tsne= TSNE(n_components=N_COMPONENTS,perplexity=PERPLEXITY, n_iter=N_ITER, verbose=0) #The perplexity is related to the number of nearest neighbors that is used in other manifold learning algorithms.
    tsne_result = tsne.fit_transform(X_tsne)
    #plotting the graphic
    plt.figure(figsize = FIGURE_SIZE)
    plt.suptitle("3D DIMENSIONALITY REDUCTION")
    ax1 = plt.subplot(1,2,1, projection = '3d') #Plotting PCA
    ax1.scatter(
        xs=pca_result[:,0] ,
        ys=pca_result[:,1] ,
        zs=pca_result[:,2] ,
        c=y, 
        alpha=0.5,
        cmap=plt.cm.get_cmap('Set3', 2)
    )
    ax1.set_xlabel('pca-one')
    ax1.set_ylabel('pca-two')
    ax1.set_zlabel('pca-three')
    ax2= plt.subplot(1,2,2,projection = '3d') #Plotting TSNE
    ax2.scatter(
        xs=tsne_result[:,0] ,
        ys=tsne_result[:,1] ,
        zs=tsne_result[:,2] ,
        c=y,
        alpha=0.5,
        cmap=plt.cm.get_cmap('Set3', 2)
    )
    ax2.set_xlabel('tsne-one')
    ax2.set_ylabel('tsne-two')
    ax2.set_zlabel('tsne-three')
    plt.show()
def feature_selection(dt_x, dt_y):
    #correlation matrix
    print("Plotting correlation matrix")           
    correlation = dt_x.corr(method ='pearson')
    correlation.to_excel("correlations.xlsx")
    plt.matshow(correlation)
    plt.show()
    #Take 7 best features
    bestfeatures = SelectKBest(score_func=mutual_info_classif, k=FEATURES_NUMBER) 
    fit = bestfeatures.fit(dt_x,dt_y)
    dfscores = pd.DataFrame(fit.scores_)
    dfcolumns = pd.DataFrame(dt_x.columns)
    featureScores = pd.concat([dfcolumns,dfscores],axis=1)
    featureScores.columns = ['Specs','Score']
    # print(featureScores.sort_values(by = 'Score', ascending = False))
    print("7 best features selected:")
    filter = bestfeatures.get_support() #Take bestfeatures element's index
    features = np.array(dt_x.columns)  
    print(features[filter])
    dt_x.drop(dt_x.columns.difference(features[filter]), axis=1, inplace=True) #delete all the features from X_train except the best ones selected by bestfeatures
    #PLOT THE SCORES
    plt.figure()
    plt.title('FEATURE SELECTION')
    plt.bar(features[filter], fit.scores_[filter], align='center')
    plt.ylabel('score')
    plt.show()
    return(features[filter])
def model_comparison(x_train,y_train,x_val,y_val):
    print("Executing classification algorithms with standard parameters...")
    #LOGISTIC REGRESSION
    logreg = LogisticRegression(max_iter = 1000) #max_iter set as a high number due to the number of data to be processed
    logreg.fit(x_train,y_train)
    y_logregpred = logreg.predict(x_val)
    logreg_acc= accuracy_score(y_val, y_logregpred)
    print("Logistic regression accuracy: " + str(round(logreg_acc, 3)))
    
    #DECISION TREE CLASSIFIER
    trclf = tree.DecisionTreeClassifier()
    trclf.fit(x_train,y_train)
    y_tree = trclf.predict(x_val)
    tree_acc = accuracy_score(y_val, y_tree)
    print("Decision Tree Classifier accuracy: " + str(round(tree_acc, 3)))

    #RANDOM FOREST
    rf_clf= RandomForestClassifier()
    rf_clf.fit(x_train,y_train)
    y_random = rf_clf.predict(x_val)
    random_acc = accuracy_score(y_val, y_random)
    print("Random forest accuracy: " + str(round(random_acc,3)))
    
    #GRADIENT BOOSTING
    boost =  GradientBoostingClassifier()
    boost.fit(x_train, y_train)
    y_boost = boost.predict(x_val)
    boost_acc = accuracy_score(y_val, y_boost)
    print("Gradient boosting accuracy: " + str(round(boost_acc, 3)))
    
    #XBOOSTING
    xboost_model = XGBClassifier(use_label_encoder=False,eval_metric = "error") #eval metric is a useless parameter in this case
    xboost_model.fit(x_train, y_train)
    y_xboost = xboost_model.predict(x_val)
    xboost_accuracy = accuracy_score(y_val,y_xboost)
    print("Xboost accuracy: " + str(round(xboost_accuracy, 3)))

def gradient_model_tuning(X_train, y_train, x_val, y_val):
    #random search with halvingrandomsearch (commented for timing optimization purpose)
    """
    print("Model tuning...\nGradient Boost random search model tuning:")
    random_parameters = {"learning_rate": [0.1, 0.01, 0.05, 0.2], #array of 3 number from 0.0 to 1.0
                           "n_estimators": [50,100,200,300,400],
                           "max_depth": [3,5,7,10],
                             
                           }
    
    random_search = HalvingRandomSearchCV(estimator=GradientBoostingClassifier(), param_distributions= random_parameters, scoring = "accuracy")
    start_random = time()
    random_search.fit(X_train, y_train)    
    print("Random search took %.2f seconds"% (time() - start_random))
    print("The best parameters across ALL searched parameters:", random_search.best_params_)
    #extract hyperparameters from HalvingRandomSearchCV
    random_learning_rate = random_search.best_params_.get("learning_rate")
    random_n_estimators = random_search.best_params_.get("n_estimators")
    random_max_depth = random_search.best_params_.get("max_depth")
    #plotting results
    results = pd.DataFrame(random_search.cv_results_)
    results["params_str"] = results.params.apply(str)
    results.drop_duplicates(subset=("params_str", "iter"), inplace=True)
    accuracy_scores = results.pivot(
        index="iter", columns="params_str", values="mean_test_score"
    )
    ax = accuracy_scores.plot(legend=False, alpha=0.6)

    labels = [
        f"iter={i}\nn_samples={random_search.n_resources_[i]}\nn_candidates={random_search.n_candidates_[i]}"
        for i in range(random_search.n_iterations_)
    ]

    ax.set_xticks(range(random_search.n_iterations_))
    ax.set_xticklabels(labels, rotation=45, multialignment="left")
    ax.set_title("Scores of candidates over iterations")
    ax.set_ylabel("mean test accuracy score", fontsize=15)
    ax.set_xlabel("iterations", fontsize=15)
    plt.tight_layout()
    plt.show()
    """
    #model tuning with BayesSearch (slightly better results than halving random search)
    parameters = { "learning_rate": [0.1,0.01,0.05],
                   "n_estimators": [100,200,500],
                    "max_depth": [3,5,8],
                   "min_samples_leaf" : [1,10,50],
                    }
    print("Executing bayes optimization...")
    bayes = BayesSearchCV(GradientBoostingClassifier(), parameters, n_iter = 32, cv=3, scoring = "accuracy") #cv Determines the cross-validation splitting strategy: 3 = stratified3fold 
    start_bayes = time()
    bayes.fit(X_train, y_train)    
    print("Bayes search took %.2f seconds"% (time() - start_bayes))
    print("val. score: %s" % bayes.best_score_)
    print("test score: %s" % bayes.score(x_val, y_val))
    print("Plotting partial dependence")
    #plotting
    plot_objective(bayes.optimizer_results_[0],
                   dimensions=["learning_rate", "n_estimators", "max_depth", "min_samples_leaf"],
                   )
    plt.show()
    print("The best parameters across ALL searched parameters:", bayes.best_params_)
    #extract hyperparameters from BayesSearchCV
    optimal_learning_rate = bayes.best_params_.get("learning_rate")
    optimal_n_estimators = bayes.best_params_.get("n_estimators")
    optimal_max_depth = bayes.best_params_.get("max_depth")
    optimal_min_samples_leaf = bayes.best_params_.get("min_samples_leaf")
    return(optimal_learning_rate, optimal_n_estimators, optimal_max_depth, optimal_min_samples_leaf)

def gradientboosting(X_train,y_train,X_test,y_test,opt_learning_rate,opt_n_estimators,opt_max_depth,opt_min_samples_leaf):
    print("Testing gradient boosting with new hyperparameters on TEST SET")
    boost =  GradientBoostingClassifier(learning_rate = opt_learning_rate, n_estimators = opt_n_estimators, max_depth = opt_max_depth, min_samples_leaf = opt_min_samples_leaf)
    boost.fit(X_train, y_train)
    y_boost = boost.predict(X_test)
    boost_acc = accuracy_score(y_test, y_boost)
    print("Gradient boosting final accuracy: " + str(round(boost_acc, 3)))
    #Plot Confusion matrix
    conf_matrix_view = ConfusionMatrixDisplay(confusion_matrix(y_test, y_boost), display_labels = ["negative", "positive"])
    conf_matrix_view.plot()
    conf_matrix_view.ax_.set(title = "Confusion matrix", xlabel = "Predicted label", ylabel = "True label")
    plt.show()

def neural_network(X_train, X_test, y_train, y_test):
    
    #1. neural network without hidden layer
    print("1. Initializing neural network without hidden layer")
    model_1 = models.Sequential()
    model_1.add(layers.Dense(7, activation='relu',input_shape=(X_train.shape[1],)))
    model_1.add(layers.Dense(1,activation='sigmoid'))
    model_1.compile(loss='binary_crossentropy', optimizer='adam',metrics = ['accuracy'])
    history_1 = model_1.fit(X_train, y_train, epochs=10, batch_size=1,validation_split=0.2, verbose=0)
    test_loss_1,test_pr_1 = model_1.evaluate(X_test,y_test,verbose = 0)
    print("Accuracy of neural network model_1: " + str(round(test_pr_1, 3)))
    #plot learning curve model 1
    plt.figure()
    plt.xlabel('Epoch')
    plt.ylabel('Loss')
    plt.plot(history_1.epoch,
    np.array(history_1.history['loss']),label='Train loss')
    plt.plot(history_1.epoch,
    np.array(history_1.history['val_loss']),label = 'Validation loss')
    plt.legend()
    plt.title("NEURAL NETWORK MODEL 1")
    plt.show()
    plt.figure()
    plt.xlabel('Epoch')
    plt.ylabel('')
    plt.plot(history_1.epoch,
    np.array(history_1.history['accuracy']),label='Train accuracy')
    plt.plot(history_1.epoch,
    np.array(history_1.history['val_accuracy']),label = 'Validation accuracy')
    plt.legend()
    plt.title("NEURAL NETWORK MODEL 1")
    plt.show()
    #2. neural network with hidden layer
    print("2. Initializing neural network with hidden layer")
    model_2 = models.Sequential()
    model_2.add(layers.Dense(7, activation='relu',input_shape=(X_train.shape[1],)))
    model_2.add(layers.Dense(5, activation='relu')) #75% input layer neurons
    model_2.add(layers.Dense(1,activation='sigmoid'))
    model_2.compile(loss='binary_crossentropy', optimizer='adam',metrics = ['accuracy'])
    history_2 = model_2.fit(X_train, y_train, epochs=10, batch_size=1,validation_split=0.2, verbose=0)
    test_loss_2,test_pr_2 = model_2.evaluate(X_test,y_test,verbose = 0)
    print("Accuracy of neural network model_2: " + str(round(test_pr_2,3)))
     #plot learning curve model 2
    plt.figure()
    plt.xlabel('Epoch')
    plt.ylabel('Loss')
    plt.plot(history_2.epoch,
    np.array(history_2.history['loss']),label='Train loss')
    plt.plot(history_2.epoch,
    np.array(history_2.history['val_loss']),label = 'Validation loss')
    plt.legend()
    plt.title("NEURAL NETWORK MODEL 2")
    plt.show()
    plt.figure()
    plt.xlabel('Epoch')
    plt.ylabel('')
    plt.plot(history_2.epoch,
    np.array(history_2.history['accuracy']),label='Train accuracy')
    plt.plot(history_2.epoch,
    np.array(history_2.history['val_accuracy']),label = 'Validation accuracy')
    plt.legend()
    plt.title("NEURAL NETWORK MODEL 2")
    plt.show()
    #3. neural network with hidden layer and Adam optimization (commented because doesn't improve well the model 2)
    """
    print("3. Initializing neural network with hidden layer and Adam optimization")
    model_3 = models.Sequential()
    model_3.add(layers.Dense(7, activation='relu',input_shape=(X_train.shape[1],)))
    model_3.add(layers.Dense(4, activation='relu')) 
    model_3.add(layers.Dense(1,activation='sigmoid'))
    opt = keras.optimizers.Adam(learning_rate=0.01)
    model_3.compile(loss='binary_crossentropy', optimizer=opt,metrics = ['accuracy'])
    history_3 = model_3.fit(X_train, y_train, epochs=10, batch_size=1,validation_split=0.2, verbose=0)
    test_loss_3,test_pr_3 = model_3.evaluate(X_test,y_test,verbose = 0)
    print("Accuracy of neural network model_3: " + str(round(test_pr_3,3)))
    plt.figure()
    plt.title("NEURAL NETWORK MODEL 3")
    plt.xlabel('Epoch')
    plt.ylabel('Accuracy')
    plt.plot(history_3.epoch,
    np.array(history_3.history['loss']),label='Train accuracy')
    plt.plot(history_3.epoch,
    np.array(history_3.history['val_loss']),label = 'Validation accuracy')
    plt.legend()
    plt.show()
    plt.figure()
    plt.xlabel('Epoch')
    plt.ylabel('')
    plt.plot(history_3.epoch,
    np.array(history_3.history['accuracy']),label='Train accuracy')
    plt.plot(history_3.epoch,
    np.array(history_3.history['val_accuracy']),label = 'Validation accuracy')
    plt.legend()
    plt.title("NEURAL NETWORK MODEL 3")
    plt.show()
    """
def main():
    df = pd.read_csv("diabete.csv")
    #df.info() #Check useful info on dataset
    y = df.pop('Diabetes_binary') #label
    X = df
    X_train, X_test, y_train, y_test = train_test_split(X,y,test_size=0.30, stratify = y)
    X_train, X_validation, y_train, y_validation = train_test_split(X_train,y_train,test_size=0.10, stratify = y_train)
    #checking new sets shapes
    #print("X_train shape: {}".format(X_train.shape) + " y_train shape: {}".format(y_train.shape) )
    #print("X_test shape: {}".format(X_test.shape) + " y_test shape: {}".format(y_test.shape))
    #print("X_validation shape: {}".format(X_validation.shape) +" y_validation shape: {}".format(y_validation.shape))
    data_analysis(X_train, y_train)
    dimensionality_reduction(X_validation,y_validation)
    best_features = feature_selection(X_train, y_train)
    X_validation.drop(X_test.columns.difference(best_features), axis=1, inplace=True) #keeping only selected features on validation-set
    X_test.drop(X_test.columns.difference(best_features), axis=1, inplace=True) #keeping only selected features on test-set
    neural_network(X_train,X_test,y_train,y_test)
    model_comparison(X_train, y_train, X_validation, y_validation)
    learning_rate, n_estimators, max_depth, min_samples_leaf = gradient_model_tuning(X_train, y_train, X_validation, y_validation)
    gradientboosting(X_train,y_train,X_test,y_test,learning_rate,n_estimators,max_depth,min_samples_leaf)
    
    
main()
