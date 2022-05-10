// pages/function.js
Page({

  data: {
    name: '',
    connectedDeviceId: ''
  },

  talk :function(){
    console.log(this.data.connectedDeviceId,this.data.name)
    wx.navigateTo({
      url: '../device/device?connectedDeviceId=' + this.data.connectedDeviceId + '&name=' + this.data.name
    })
  },

  button :function(){
    console.log(this.data.connectedDeviceId,this.data.name)
    wx.navigateTo({
      url: '../button/button?connectedDeviceId=' + this.data.connectedDeviceId + '&name=' + this.data.name
    })
  },

  alarm :function(){
    
  },

  onLoad: function (options) {
    var that = this
    console.log(options)
    that.setData({
      name: options.name,
      connectedDeviceId: options.connectedDeviceId
    })
  },

  onReady: function () {

  },

  onShow: function () {

  },

  onHide: function () {

  },

  onUnload: function () {

  },

  onPullDownRefresh: function () {

  },

  onReachBottom: function () {

  },

  onShareAppMessage: function () {

  }
})
