import numpy
from sktime.classification.kernel_based import RocketClassifier

def train(X_train, y_train, X_test, y_test):
    clf = RocketClassifier(num_kernels=500, use_multivariate="yes")
    clf.fit(X_train, y_train)
    acc = numpy.sum(clf.predict(X_test) - y_test)/len(y_test)
    print(acc)

