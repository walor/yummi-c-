using System;

namespace Neural_Network_1
{
	public class MyNetwork
	{
		private int i;
		private int o;
		private int h;

		private double LH=0.15f;
		private double LO=0.2f;
		
		private double []inputsToNetwork;
		private double []desiredOutputs;

		private node []input; 
		private node []output;
		private node []hidden;

		private double total_error;
		public double error_compared_to_tolerance=0;
		
		/// <summary>
		/// this function to calculate the sigmoid of a double
		/// </summary>
		/// <param name="x">double number</param>
		/// <returns></returns>
		private double sigmoid(double x)
		{
			return 1/(1+Math.Exp(-x));
			
		}


		#region these functions must be called before calling training

		/// <summary>
		/// Constructor of the neural network
		/// </summary>
		/// <param name="inp">integer : number of input neurons</param>
		/// <param name="hide">integer : number of hidden units neurons</param>
		/// <param name="outp">integer : number of output neurons</param>
		public MyNetwork(int inp , int hide , int outp)
		{
			i = inp ; o = outp ; h = hide;
			int ahm = 0;
			input = new node[i];
			output = new node[o];
			hidden = new node[h];
			Random rand = new Random(unchecked((int)DateTime.Now.Ticks)); 

			for(int x=0 ; x<inp ; x++)
			{
				input[x] = new node();
				input[x].weights = new double[this.h];
				for(int j=0 ; j<hide ; j++)
				{
					ahm =  rand.Next()& 1; 
					input[x].weights[j] = rand.NextDouble() ;
					if(ahm == 0)
						input[x].weights[j] *= -1;
				}

			}
			for(int y=0; y<hide ; y++)
			{
				hidden[y] = new node();
				hidden[y].weights = new double[this.o];
				for(int j=0 ; j<outp ; j++)// the loop for initializing the weights
					hidden[y].weights[j] = rand.NextDouble();
			}
			for(int z=0 ; z<outp ; z++)
			{
				output[z] = new node();
			}
		}
		

		/// <summary>
		/// This function to set the threshold of the neruons in the hidden layer and the output layer
		/// </summary>
		public void FirstTimeSettings()
		{
			Random x = new Random(unchecked((int)DateTime.Now.Ticks)); 
			for(int i=0 ; i<h ; i++)
				hidden[i].Threshold = x.NextDouble();
			for(int i=0 ; i<o ; i++)
				output[i].Threshold = x.NextDouble();
		}
		

		/// <summary>
		/// This function passes training data to the network , inputs and desired ouptus
		/// </summary>
		/// <param name="list"></param>
		public void BeforeTraining(params double []list)
		{
			int counter=0;
			int j=0;
			int k=0;
			inputsToNetwork = new double[this.i];
			desiredOutputs = new double[this.o];

			while(counter < list.Length)
			{
				if( counter < this.i)
				{
					inputsToNetwork[j] = list [counter];
					j++;
				}
				else
				{
					desiredOutputs[k] = list[counter];
					k++;
				}
				counter++;
			}



		}

		/// <summary>
		/// This function passes training data to the network , inputs and desired ouptus
		/// </summary>
		/// <param name="list"></param>
		public void TraiMyNetwork(double[,] inputlist, double[,] outputlist, int iterations, ProgressBar progressBar1)
		{
			inputsToNetwork = new double[this.i];
			desiredOutputs = new double[this.o];

			int outputlistSampleLength = outputlist.GetUpperBound(0) + 1;
			int outputlistLength = outputlist.GetUpperBound(1) + 1;
			int inputlistLength = inputlist.GetUpperBound(1) + 1;

			progressBar1.Minimum = 0;
			progressBar1.Maximum = iterations;
			progressBar1.Value = 0;

			for (int i = 0; i < iterations; i++)
			{
				progressBar1.Value ++;
				for (int sampleindex = 0; sampleindex < outputlistSampleLength; sampleindex++)
				{
					for (int j = 0; j < inputlistLength; j++)
					{
						inputsToNetwork[j] = inputlist[sampleindex, j];
					}

					for (int k = 0; k < outputlistLength; k++)
					{
						desiredOutputs[k] = outputlist[sampleindex, k];
					}

					Training_for_one_pattern();
				}
			}
		}

		void PopulateInputList(double[,] inputlist, int index)
		{
				for (int j = 0; j < inputlist.GetUpperBound(1) + 1; j++)
				{
					inputsToNetwork[j] = inputlist[index, j];
				}
		}

		/// <summary>
		/// test the network after training
		/// </summary>
		public string Test_Network(int index, double[,] inputlist)
		{
			PopulateInputList(inputlist, index);
			this.Calc_Activation();
			string temp = this.print_output();
			return temp;
		}


		/// <summary>
		/// the function that trains the network
		/// </summary>
		/// 
		public void Training_for_one_pattern()
		{
			this.Calc_Activation();
			this.Calc_error_output();
			this.Calc_error_hidden();
			this.Calc_new_Thresholds();
			this.Calc_new_weights_in_hidden();
			this.Calc_new_weights_in_input();
		}



		
		#endregion

		/// <summary>
		/// a function to calculate the activation of the hidden layer and the output layer
		/// </summary>
		private void Calc_Activation()
		{
			int ch=0;
			while(ch<this.h)
			{
				for(int ci=0 ; ci<this.i ; ci++)
					hidden[ch].Activation += inputsToNetwork[ci] * input[ci].weights[ch];

				ch++;
			}

			for(int x=0 ; x<this.h ; x++)
			{
				hidden[x].Activation += hidden[x].Threshold ;
				hidden[x].Activation = sigmoid(hidden[x].Activation );
			}

			int co=0;
			while(co<this.o)
			{
				for(int chi=0 ; chi<this.h ; chi++)
					output[co].Activation += hidden[chi].Activation  * hidden[chi].weights[co];

				co++;
			}

			for(int x=0 ; x<this.o ; x++)
			{
				output[x].Activation += output[x].Threshold ;
				output[x].Activation = sigmoid(output[x].Activation );
			}


		}

		
		/// <summary>
		/// print output of the network
		/// </summary>
		public string print_output()
		{
			string outp="";
			for(int x=0 ; x<this.o ; x++)
			{
				if(output[x].Activation>0.5)
					outp += "1" + " ";
				else
					outp += "0" + " " ;
			}
			return outp;

		}


		/// <summary>
		/// a function to calculate the error of each output neuron
		/// </summary>
		private void Calc_error_output()
		{
			for(int x=0 ; x<this.o ; x++)
				output[x].error = output[x].Activation * (1 - output[x].Activation ) * (desiredOutputs[x] - output[x].Activation );
		}


		/// <summary>
		/// a function to calculate the error of each hidden neuron
		/// </summary>
		private void Calc_error_hidden()
		{
			int y=0;
			while(y<this.h)
			{
				for(int x=0 ; x<this.o ; x++)
				{
					hidden[y].error += hidden[y].weights[x] * output[x].error ; 
				}
				hidden[y].error *= hidden[y].Activation * (1 - hidden[y].Activation );
				y++;
			}

		}

		
		/// <summary>
		/// a function to calculate the new thresholds for each neuron
		/// </summary>
		private void Calc_new_Thresholds()
		{
			for(int x=0 ; x<this.h ; x++)
				hidden[x].Threshold +=  hidden[x].error * this.LH ;
			for(int y=0 ; y<this.o ; y++)
				output[y].Threshold += output[y].error * this.LO;
		}


		/// <summary>
		/// a function to calculate the new weights between hidden and output
		/// </summary>
		private void Calc_new_weights_in_hidden()
		{
			int x=0;
			double temp=0.0f;
			while(x < this.h)
			{
				temp = hidden[x].Activation * this.LO;
				for(int y=0 ; y<this.o ; y++)
				{
					hidden[x].weights[y] += temp * output[y].error;
				}
				x++;
			}
		}


		/// <summary>
		/// a function to calculate the new weights between input and hidden
		/// </summary>
		private void Calc_new_weights_in_input()
		{
			int x=0;
			double temp=0.0f;
			while(x < this.i)
			{
				temp = inputsToNetwork[x] * this.LH;
				for(int y=0 ; y<this.h ; y++)
				{
					input[x].weights[y] += temp * hidden[y].error;
				}
				x++;
			}

		}

		
		/// <summary>
		/// the function that returns the total error
		/// </summary>
		/// <returns></returns>
		public double Calc_total_error_in_pattern()
		{
			double temp=0.0f;
			for(int x=0 ; x<this.o ; x++)
				temp+=output[x].error;
			return this.total_error;
		}


		/// <summary>
		/// set total error = 0 after one cycle
		/// </summary>
		public void reset_total_error()
		{
			this.total_error = 0;
		}

	}
}
