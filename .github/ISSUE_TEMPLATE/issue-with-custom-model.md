---
name: Issue with custom model
about: Ask for help if you have issue with your model
title: ''
labels: ''
assignees: ''

---

> **Please do not disclose security vulnerabilities as issues. See our [security policy](../../SECURITY.md) for responsible disclosures.**

### Before opening an issue
#### Issue Checklist

- [ ] I have checked the [compatibility table](https://axiscommunications.github.io/acap-documentation/docs/api/computer-vision-sdk-apis.html#compatibility) in the documentation.
- [ ] I have verified that the SDK version I am using is compatible with the firmware version installed on my device.
- [ ] I have included the SDK version and firmware version in my issue description for reference.
- [ ] I have followed the minimum debug steps on my own.
- [ ] I have included the output of the debug steps in the issue.

**Minimum Debug Steps**
Before opening the issue, try to load the model on the device using this command:
`larod-client -g model_path  -c <CHIP_ID> `

Where <CHIP_ID> is:
'cpu-tflite' - To test on CPU
'axis-a8-dlpu-tflite' - To test on Artpec-8 DLPU
'google-edge-tpu-tflite' - To test on Artpec-7 TPU
'ambarella-cvflow'- To test on CV25 DLPU
    
If it fails run
`journalctl -u larod`
Please provide us this log together with your issue.

### Describe the bug

Explain the behavior you would expect and the actual behavior.

### To reproduce

Please provide as much context as possible and describe the *reproduction steps* that someone else can follow to recreate the issue on their own.

A team member will try to reproduce the issue with your provided steps. If there are no reproduction steps or no obvious way to reproduce the issue, the team will ask you for those steps. Bugs without steps will not be addressed until they can be reproduced.

Steps to reproduce the behavior:

1. Set up '...'
2. Do this '...'
3. See error

### Screenshots

If applicable, add screenshots to help explain your problem.

### Environment

- Axis device model: [e.g. Q1615 Mk III]
- Axis device firmware version: [e.g. 10.5]
- Stack trace or logs: [e.g. Axis device system logs]
- OS and version: [e.g. macOS v12.2.1, Ubuntu 20.04 LTS, Windows build 21996.1]
- Version: [version of the application, SDK, runtime environment, package manager, interpreter, compiler, depending on what seems relevant]

### Additional context

Add any other context about the problem here.
