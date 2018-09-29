using System;

namespace Neural_Network_1
{
	public class node
	{
		private double activation;
		private double threshold;
		public double []weights;
		public double error;

		public node()
		{
			this.activation = 0;
			this.error = 0;
		}
		
		public node(double act,double thr,int numOfweights)
		{
			this.activation = act;
			this.threshold = thr;
			this.weights = new double[numOfweights];
		}

		public double Activation
		{
			set	{activation = value ;}
			get {return activation;}
		}

		public double Threshold
		{
			set {threshold = value;}
			get {return threshold;}
		}
	}

}
