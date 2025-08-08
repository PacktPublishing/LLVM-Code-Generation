<h1 align="center">LLVM Code Generation, First Edition</h1>

<h2 align="center">A deep dive into compiler backend development</h2>

<p align="center">Quentin Colombet</p>

<p align="center">This is the code repository for <a href ="https://www.packtpub.com/en-us/product/how-to-build-an-llvm-backend-9781835462577"> LLVM Code Generation, First Edition</a>, published by Packt.</p>


<p align="center">
   <a href="https://discord.gg/uCFXnmbv" alt="Discord" title="Learn more on the Discord server"><img width="32px" src="https://cliply.co/wp-content/uploads/2021/08/372108630_DISCORD_LOGO_400.gif"/></a>
  &#8287;&#8287;&#8287;&#8287;&#8287;
  <a href="https://packt.link/free-ebook/9781837637782"><img width="32px" alt="Free PDF" title="Free PDF" src="https://cdn-icons-png.flaticon.com/512/4726/4726010.png"/></a>
 &#8287;&#8287;&#8287;&#8287;&#8287;
  <a href="https://packt.link/gbp/9781837637782"><img width="32px" alt="Graphic Bundle" title="Graphic Bundle" src="https://cdn-icons-png.flaticon.com/512/2659/2659360.png"/></a>
  &#8287;&#8287;&#8287;&#8287;&#8287;
   <a href="https://www.amazon.com/LLVM-Code-Generation-Compiler-Development/dp/1837637784"><img width="32px" alt="Amazon" title="Get your copy" src="https://cdn-icons-png.flaticon.com/512/15466/15466027.png"/></a>
  &#8287;&#8287;&#8287;&#8287;&#8287;
</p>
<details open> 
  <summary><h2>About the book</summary>
<a href="https://www.packtpub.com/en-us/product/how-to-build-an-llvm-backend-9781835462577">
<img src="https://content.packt.com/_/image/original/B22046/cover_image.jpg?version=1747376116" alt="LLVM Code Generation, First Edition" height="256px" align="right">
</a>

The LLVM infrastructure is a popular compiler ecosystem widely used in the tech industry and academia. This technology is crucial for both experienced and aspiring compiler developers looking to make an impact in the field. Written by Quentin Colombet, a veteran LLVM contributor and architect of the GlobalISel framework, this book provides a primer on the main aspects of LLVM, with an emphasis on its backend infrastructure; that is, everything needed to transform the intermediate representation (IR) produced by frontends like Clang into assembly code and object files.
You’ll learn how to write an optimizing code generator for a toy backend in LLVM. The chapters will guide you step by step through building this backend while exploring key concepts, such as the ABI, cost model, and register allocation. You’ll also find out how to express these concepts using LLVM's existing infrastructure and how established backends address these challenges. Furthermore, the book features code snippets that demonstrate the actual APIs.
By the end of this book, you’ll have gained a deeper understanding of LLVM. The concepts presented are expected to remain stable across different LLVM versions, making this book a reliable quick reference guide for understanding LLVM.</details>
<details open> 
  <summary><h2>Key Learnings</summary>
<ul>

<li>Understand essential compiler concepts, such as SSA, dominance, and ABI</li>

<li>Build and extend LLVM backends for creating custom compiler features</li>

<li>Optimize code by manipulating LLVM's Intermediate Representation</li>

<li>Contribute effectively to LLVM open-source projects and development</li>

<li>Develop debugging skills for LLVM optimizations and passes</li>

<li>Grasp how encoding and (dis)assembling work in the context of compilers</li>

<li>Utilize LLVM's TableGen DSL for creating custom compiler models</li>

</ul>

  </details>

<details open> 
  <summary><h2>Chapters</summary>
     <img src="https://cliply.co/wp-content/uploads/2020/02/372002150_DOCUMENTS_400px.gif" alt="LLVM Code Generation, First Edition" height="556px" align="right">
<ol>

  <li>Building LLVM and Understanding the Directory Structure</li>

  <li>Contributing to LLVM</li>

  <li>Compiler Basics and How They Map to LLVM APIs</li>

  <li>Writing Your First Optimization</li>

  <li>Dealing with Pass Managers</li>

  <li>TableGen – LLVM Swiss Army Knife for Modeling</li>

  <li>Understanding LLVM IR</li>

  <li>Survey of the Existing Passes</li>

  <li>Introducing Target-Specific Constructs</li>

  <li>Hands-On Debugging LLVM IR Passes</li>

  <li>Getting Started with the Backend</li>

  <li>Getting Started with the Machine Code Layer</li>

  <li>The Machine Pass Pipeline</li>

  <li>Getting Started with Instruction Selection</li>

  <li>Instruction Selection: The IR Building Phase</li>

  <li>Instruction Selection: The Legalization Phase</li>

  <li>Instruction Selection: The Selection Phase and Beyond</li>

  <li>Instruction Scheduling</li>

  <li>Register Allocation</li>

  <li>Lowering of the Stack Layout</li>

  <li>Getting Started with the Assembler</li>

</ol>

</details>


<details open> 
  <summary><h2>Requirements for this book</summary>
To follow the instructions in this book, you need LLVM 20 installed on your system, running on Windows, macOS, or Linux operating systems.

Navigate in the different `chX` directory and look at the examples provided and do the exercises when applicable.
Each directory has its own README.md with specific directions.

Note:
The exercises have been tested with the open source repository of LLVM at the Git hash 424c2d9b7e4d from February 13th 2025. Which is LLVM 20.1.1.

Some of the exercises interact directly with the LLVM C++ API. This API has no stability guarantee therefore it is possible that newer or older version of LLVM will not work with these exercises.

For the exercices that requires a version of LLVM handy, if you build your own make sure to use the `CMAKE_INSTALL_PREFIX` cmake variable to set the install path, then build the `install` target.

Then, you will need to provide this path to CMake in the different exercise.

Follow the READMEs in the different directories when you get there.
  </details>

<details> 
  <summary><h2>Get to know the author</h2></summary>

_Quentin Colombet_ is a veteran LLVM contributor specializing in compiler backends. He is the architect of the new instruction selection framework (GlobalISel) and code owner of the LLVM register allocators. With over two decades of experience, he has worked on compiler backends for a variety of architectures, including GPU, CPU, microcontrollers, DSP, and ASICs. Quentin joined Apple in 2012 and has contributed to x86, Aarch64, and Apple GPU backends. He is passionate about helping newcomers onboard the LLVM infrastructure, having mentored interns and new hires over the years.
</details>

<details> 
  <summary><h2>Other Related Books</h2></summary>
<ul>

  <li><a href="https://www.packtpub.com/en-us/product/learn-llvm-17-second-edition/9781837631346">Learn LLVM 17, Second Edition</a></li>

  <li><a href="https://www.packtpub.com/en-us/product/llvm-techniques-tips-and-best-practices-clang-and-middle-end-libraries-first-edition/9781838824952">LLVM Techniques, Tips, and Best Practices Clang and Middle-End Libraries, First Edition</a></li>

  <li><a href="https://www.packtpub.com/en-us/product/build-your-own-programming-language-second-edition/9781804618028">Build Your Own Programming Language, Second Edition</a></li>

  <li><a href="https://www.packtpub.com/en-us/product/c-memory-management-first-edition/9781805129806">C++ Memory Management, First Edition</a></li>
 
</ul>

</details>


## Errata

* Page 6: Under the heading _Identifying the right version of the tools_, in step 1 the hyperlink on the URL [https://releases.llvm.org/] in the digital formats redirects to [https://www.python.org/downloads/]. Please copy and paste the link [https://releases.llvm.org/] in the browser to navigate to the correct webpage.
* Page 11: In the command `$ git clone https://github.com/llvm/llvm/project.git`, the URL should be `https://github.com/llvm/llvm-project.git`. Therefore, the first line becomes `$ git clone https://github.com/llvm/llvm-project.git`.
* Page 31: In the command `$ cmake –GNinja -DCMAKE_C_COMPILER=${BUILD_DIR}/bin/clang ${TESTSUITE_DIR}`, `–` should be replaced with `-`. In the command `$ cmake -GNinja -DCMAKE_C_COMPILER=${BUILD_DIR}/bin/clang -C${TESTSUITE_DIR}/cmake/cache/<specificOption>.cmake ${TESTSUITE_DIR}`, `cache` should be replaced with `caches`. Same to `/cmake/cache/O3.cmake`.
* Page 71: In the descriptipn below _Figure 3.4_, the sentence "Because of that, inserting a store in A and **reloading in B** means that the whole dotted region needs to play nicely with this memory location." should be "Because of that, inserting a store in A and **reloading in C** means that the whole dotted region needs to play nicely with this memory location."
* Page 71: The sentence "At the IR level, you can check if an edge is critical using the `isCriticalEdge` function from the `IR` library." should be "At the IR level, you can check if an edge is critical using the `isCriticalEdge` function from the `Analysis` library.". (Library name is changed from `IR` to `Analysis`.)
* Page 80: In the C program in first question under _Quiz_, `res` should be declared as `int res = 0;` after the first line (`int foo(int b) {`) and before the `for` loop. Also, in the following diagram, there should be an additional arrow from the top rectangle going to the bottom most rectangle, so that it reflects the case where the loop is not entered and the function directly returns `res`.
* Page 86: In the last paragraph first sentence "Using the same code snippet as the previous section, a frontend generates an IR that resembles what is depicted in _Figure 4.2_", the correct image reference is _Figure 4.1_.
* Page 92: In Figure 4.5, the labels on the left side should just be `getOperand(0)` and `getOperand(1)`. In both the lines, `getopenand(0) =>` and `getopenand(n) =>` are to be disregarded. 
* Page 106: In _Figure 4.6_, the block at the center labelled as "excluding" should be "exiting".
* Page 361: The term `MCInstrPrinter` should be `MCInstPrinter`.
* Page 363: Both the instances of the term `XXXInstrPrinter` should be `XXXInstPrinter`.
* Page 455: In _Table 17.1_, under the **Original code** column on the left side, the line `%vec1 = insertelement <2 x i32> %vec, i32 %a, i32 1` should be `%vec1 = insertelement <2 x i32> %vec, i32 %b, i32 1` (i.e. **%a** should be **%b**).
* Page 504: The sentence "The bindings are simple instantiations of the `ReadAdvance` and `WriteRes` classes or instantiations of `InstR`." should be "The bindings are simple instantiations of the `ReadAdvance` and `WriteRes` classes or instantiations of `InstRW`.". (`InstR` is changed to `InstRW`.)
