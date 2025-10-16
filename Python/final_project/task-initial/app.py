from __future__ import annotations
import streamlit as st
from joblib import load
import numpy as np
from numpy.typing import ArrayLike
import matplotlib.pyplot as plt

def load_and_predict(X: ArrayLike, filename: str = "linear_regression_model.joblib") -> ArrayLike:
    X_arr = np.asarray(X)
    model = load(filename)
    y = model.predict(X_arr)
    return y

def create_streamlit_app():
    st.title("Simple Regression model prediction")

    input_feature = st.slider(
        "Input Feature for Prediction",
        min_value=-3.0,
        max_value=3.0,
        value=0.0,
        step=0.01
    )

    if st.button("Predict value"):
        y_pred = load_and_predict([[input_feature]])
        pred_val = float(np.asarray(y_pred).ravel()[0])

        st.write(f"Prediction: {pred_val:.5f}")
        visualize_difference(input_feature, pred_val)

def visualize_difference(input_feature: float, prediction: ArrayLike):
    X_filename = "X.joblib"
    y_filename = "y.joblib"

    X = load(X_filename)

    y = load(y_filename)

    actual_target = y[_index_of_closest(X, input_feature)]

    difference = actual_target - prediction

    fig = plt.figure(figsize=(6,4))
    plt.scatter(np.ravel(X), y, color='grey', alpha=0.8, label="Dataset")
    plt.scatter([input_feature], [actual_target], color='blue', label="Actual Target")
    plt.scatter([input_feature], [prediction], color='red', label="Predicted Target")

    plt.legend(loc="upper left")
    plt.title("Prediction vs Actual Target")
    plt.xlabel("Feature")
    plt.ylabel("Target")
    plt.grid(True, linestyle='--', alpha=0.5)

    plt.plot([input_feature, input_feature], [actual_target, prediction], 'k--')
    mid_y = (actual_target + prediction) / 2.0
    plt.annotate(f"Difference = {difference:.2f}",
                (input_feature, mid_y), xytext=(10, 10),
                textcoords='offset points')

    st.pyplot(fig)

def _index_of_closest(X: ArrayLike, k: float) -> int:
    X = np.asarray(X)
    idx = (np.abs(X - k)).argmin()
    return idx


if __name__ == '__main__':
    create_streamlit_app()