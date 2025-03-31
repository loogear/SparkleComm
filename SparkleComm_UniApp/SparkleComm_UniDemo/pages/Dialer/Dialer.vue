<template>
	<view class="index">
		<view>当前登录账号：{{currentUser.account}}</view>
		<view>当前登录密码：{{currentUser.password}}</view>
		<view @click="handleScan"
			style="background-color: #42b983;padding:5px 10px;border-radius: 5px;margin:10px 0;color:#fff;font-size: 14px;">
			扫码输入账号密码</view>
		<view class="row" style="display: flex;align-items: center;margin:10px 0">
			<view class="label">呼叫号码：</view>
			<input v-model="callNumber" style="border:1px solid #ccc" type="text" name="" id="">
		</view>
		<view class="uni-common-mt">
			<view class="uni-form-item uni-column">
				<view class="title">{{state}}</view>
			</view>
		</view>

		<view class="tags">
			<block v-for="value in data" :key="value.key">
				<view class="tag" @tap="goList(value)">
					<image class="tag-img" :src="value.icon"></image>
					<text class="tag-text">{{value.type}}</text>
				</view>
			</block>
			<view class="text-box" scroll-y="true">
				<text>{{text}}</text>
			</view>
		</view>

	</view>
</template>

<script>
	const modal = uni.requireNativePlugin('modal');
	const PhoneSdk = uni.requireNativePlugin('SparkleComm_Plugin');

	export default {
		data() {
			return {
				currentUser: {
					account: '15255555555',
					password: '0J072n75Jyt2hLvGdarPFw=='
				},
				callNumber: null,
				state: "离线",
				text: [
					'日志...'
				],
				logLine: [],
				data: [{
						type: '注册',
						id: 1,
						key: 1,
						icon: 'http://placehold.it/150x150'
					},
					{
						type: '注销',
						id: 2,
						key: 2,
						icon: 'http://placehold.it/150x150'
					},
					{
						type: '拨打',
						id: 3,
						key: 3,
						icon: 'http://placehold.it/150x150'
					},
					{
						type: '视频',
						id: 4,
						key: 4,
						icon: 'http://placehold.it/150x150'
					},
					{
						type: 'Todo',
						id: 5,
						key: 5,
						icon: 'http://placehold.it/150x150'
					},
					{
						type: 'Todo',
						id: 6,
						key: 6,
						icon: 'http://placehold.it/150x150'
					},
					{
						type: '清除',
						id: 7,
						key: 7,
						icon: 'http://placehold.it/150x150'
					},
					{
						type: 'Todo',
						id: 8,
						key: 8,
						icon: 'http://placehold.it/150x150'
					}
				]
			}
		},
		onLoad() {
			this.InitSdk();
			// this.RegisterToSipServer();
		},
		methods: {
			handleScan() {
				uni.scanCode({
					scanType: ['qrCode'],
					success:(res)=> {
						try {
							const [account, password] = res.result.split(' ')
							console.log(account,password)
							if (account && password) {
								this.currentUser.account = account.trim();
								this.currentUser.password = password.trim();
							} else {
								uni.showToast({
									title: '格式错误',
									icon: 'none'
								})
							}
						} catch (error) {
							uni.showToast({
								title: '格式错误',
								icon: 'none'
							})
						}
					}
				});
			},
			InitSdk() { // 初始化SDK
				// console.log(PhoneSdk,'InitSdk')
				PhoneSdk.InitSdk(result => { // VOIP相关的事件将通过result回调
					const msg = JSON.stringify(result);
					// console.log("VOIP Event:" + msg);
					//console.log("VOIP Event:" + result.type + " accid=" + result.accid.toString());
					switch (result.type) {
						case 'onSipConnected': // SIP服务器注册成功
							console.log("VOIP注册成功");
							this.state = "在线";
							break;
						case 'onSipDisconnected': // SIP注销
							console.log("VOIP已经注销");
							this.state = "离线";
							break;
						case 'onCallTrans': // 呼叫转移
							console.log("VOIP呼叫转移");
							uni.navigateTo({
								url: "/pages/Contacts/SelectContacts/SelectContacts?CallID=" + result
									.CallID + "&" + "AccountID=" + result.AccountID
							})
							break;
					}
				});
			},
			RegisterToSipServer() { // 注册SIP服务器
				if (!this.currentUser.account || !this.currentUser.password) {
					uni.showToast({
						title: '请扫入账号密码',
						icon: 'none'
					})
					return
				}
				PhoneSdk.RegisterToSipServer({
					serverAddr: "as3.loogear.com",
					port: 41825,
					userID: this.currentUser.account,
					userPWD: this.currentUser.password,
					accountID: -2,
					domain: "system.loogear.com",
					authWithDomain: false,
					idUri: "",
					useIMS3GPP: false,
					extraHeads: ""
				}, result => {
					const msg = JSON.stringify(result);
					console.log("RegisterToSipServer:result=" + msg);
				});
			},
			UnRegister() { // 注销SIP服务器
				PhoneSdk.UnRegister({
					accountID: 0
				}, result => {
					const msg = JSON.stringify(result);
					console.log("UnRegister:result=" + msg);
				});
			},
			MakeCall(number, video) { // 拨打电话
				console.log("MakeCall:number=" + number + " video=" + video);
				PhoneSdk.MakeCall({
					number: number,
					video: video,
					accountID: 0
				}, result => {
					const msg = JSON.stringify(result);
					console.log("MakeCall:result=" + msg);
				});
			},
			goList(value) {
				console.log('type=' + value.type + " id=" + value.id)
				if (value.id == 1) { // 注册
					this.RegisterToSipServer();
				} else if (value.id == 2) { // 注销
					this.UnRegister();
				} else if (value.id == 3) {
					if (!this.callNumber) {
						uni.showToast({
							title: '请输入呼叫账号',
							icon: 'none'
						})
						return
					}
					this.logLine.push('拨打\n');
					this.MakeCall(this.callNumber, false);
				} else if (value.id == 4) {
					if (!this.callNumber) {
						uni.showToast({
							title: '请输入呼叫账号',
							icon: 'none'
						})
						return
					}
					this.logLine.push('视频\n');
					this.MakeCall(this.callNumber, true);
				} else if (value.id == 5) {} else if (value.id == 6) {} else if (value.id == 7) { // 清除日志
					while (this.logLine.length > 0) {
						this.logLine.pop();
					}
					this.text = this.logLine.join('');
				} else {
					this.logLine.push('type=' + value.type + " id=" + value.id + "\n");
					this.text = this.logLine.join('');
				}

				this.text = this.logLine.join('');
			},

		}
	}
</script>

<style>

</style>